#include "VirtualMemory.h"

// ****************** Macros ******************

#define GET_OFFSET(NUM) NUM & ((uint64_t) (pow(2,OFFSET_WIDTH) - 1))

// ******* Inner functions - definitions *******

/** Translates virtual addresses into physical */
uint64_t get_physical_address(uint64_t virtualAddress);

/** Traverse the tree of memory in order to find a free frame using the priorities given with the exercise */
word_t traverse(uint64_t cur_address, word_t cur_frame, uint64_t page_number);

/** A recursive helper for the traverse function */
bool recursive_helper(word_t cur_frame_index,
                      uint64_t father_frame_address,
                      int recursion_depth,
                      word_t* zero_table_frame_index,
                      int* max_frame_index,
                      uint64_t* max_cyclic_dist,
                      uint64_t* page_with_max_cyclic_dist_page_index,
                      word_t* page_with_max_cyclic_dist_frame_index,
                      uint64_t* page_with_max_cyclic_dist_father_frame_address,
                      word_t origin_frame_index,
                      uint64_t origin_page_index,
                      uint64_t cur_page_index);

/** Resetting (filling zeros) a given frame */
void resetFrame(word_t table_frame_ind);

/** Calculate the cyclic distance between 2 pages according to the formula defined in the exercise */
uint64_t calc_cyclic_dist(uint64_t page_1, uint64_t page_2);

 // ********************* API *********************

void VMinitialize(){
     resetFrame(0);
};

int VMread(uint64_t virtualAddress, word_t* value){
    if (virtualAddress >= VIRTUAL_MEMORY_SIZE){return 0;}
    uint64_t page_address = get_physical_address(virtualAddress);
    PMread(page_address, value);
    return 1;
}

int VMwrite(uint64_t virtualAddress, word_t value){
    if (virtualAddress >= VIRTUAL_MEMORY_SIZE){return 0;}
    uint64_t page_address = get_physical_address(virtualAddress);
    PMwrite(page_address, value);
    return 1;
}

// ******* Inner functions - Implementations *******

uint64_t get_physical_address(uint64_t virtualAddress){
    int cur_bit = OFFSET_WIDTH * TABLES_DEPTH;
    uint64_t cur_address = 0;
    word_t next_frame_index, cur_frame_index = 0;
    uint64_t page_number = virtualAddress >> OFFSET_WIDTH;
    while (cur_bit > 0){
        cur_address = (uint64_t) (cur_frame_index * PAGE_SIZE) + (GET_OFFSET(virtualAddress >> cur_bit));
        PMread(cur_address, &next_frame_index);
        if (next_frame_index == 0){
            next_frame_index = traverse(cur_address, cur_frame_index, page_number);
            if (cur_bit == OFFSET_WIDTH){
                PMrestore(next_frame_index, page_number);
            }
        }
        cur_frame_index = next_frame_index;
        cur_bit -= OFFSET_WIDTH;
    }
    return (uint64_t) (next_frame_index * PAGE_SIZE) + (GET_OFFSET(virtualAddress));
}

word_t traverse(uint64_t cur_address, word_t cur_frame, uint64_t page_number) {
    // Priority 1: if we found empty page
    word_t zero_table_frame_index = 0;

    // Priority 2: If we didn't reach the maximum number of pages.
    int max_frame_index = 0;

    // Priority 3:
    int recursion_depth = 0;
    word_t page_with_max_cyclic_dist_frame_index = 0;
    uint64_t page_with_max_cyclic_dist_page_index = 0;
    uint64_t page_with_max_cyclic_dist_father_frame_address = 0;
    uint64_t max_cyclic_dist = 0;

    recursive_helper(0,
                     page_with_max_cyclic_dist_father_frame_address,
                     recursion_depth,
                     &zero_table_frame_index,
                     &max_frame_index,
                     &max_cyclic_dist,
                     &page_with_max_cyclic_dist_page_index,
                     &page_with_max_cyclic_dist_frame_index,
                     &page_with_max_cyclic_dist_father_frame_address,
                     cur_frame,
                     page_number,
                     0);

    int new_frame_index;

    if (zero_table_frame_index != 0){
        new_frame_index = zero_table_frame_index;
    } else if (max_frame_index + 1 < NUM_FRAMES){
        new_frame_index = max_frame_index + 1;
    } else {
        new_frame_index = page_with_max_cyclic_dist_frame_index;
        PMevict(page_with_max_cyclic_dist_frame_index, page_with_max_cyclic_dist_page_index);
        PMwrite(page_with_max_cyclic_dist_father_frame_address, 0);
    }
    resetFrame(new_frame_index);
    PMwrite(cur_address, new_frame_index);
    return new_frame_index;
}

bool recursive_helper(word_t cur_frame_index,
               uint64_t father_frame_address,
               int recursion_depth,
               word_t* zero_table_frame_index,
               int* max_frame_index,
               uint64_t* max_cyclic_dist,
               uint64_t* page_with_max_cyclic_dist_page_index,
               word_t* page_with_max_cyclic_dist_frame_index,
               uint64_t* page_with_max_cyclic_dist_father_frame_address,
               word_t origin_frame_index,
               uint64_t origin_page_index,
               uint64_t cur_page_index) {

    // Update max frame index
    if (cur_frame_index > *max_frame_index) {
        *max_frame_index = cur_frame_index;
    }

    // Reached a leaf
    if (recursion_depth == TABLES_DEPTH){
        uint64_t cyclic_dist = calc_cyclic_dist(origin_page_index, cur_page_index);
        if (cyclic_dist > *max_cyclic_dist){
            *max_cyclic_dist = cyclic_dist;
            *page_with_max_cyclic_dist_page_index = cur_page_index;
            *page_with_max_cyclic_dist_frame_index = cur_frame_index;
            *page_with_max_cyclic_dist_father_frame_address = father_frame_address;
        }
        return false;
    }

    // Handle table stage
    uint64_t cur_frame_address = cur_frame_index * PAGE_SIZE;
    int num_of_zero_rows = 0;
    for (int i = 0; i < PAGE_SIZE; ++i){
        word_t son_frame_index = 0;
        uint64_t son_frame_address = 0;
        PMread(cur_frame_address + i, &son_frame_index);
        if (son_frame_index != 0) {
            bool is_son_empty = recursive_helper(son_frame_index,
                                          cur_frame_address + i,
                                          recursion_depth + 1,
                                          zero_table_frame_index,
                                          max_frame_index,
                                          max_cyclic_dist,
                                          page_with_max_cyclic_dist_page_index,
                                          page_with_max_cyclic_dist_frame_index,
                                          page_with_max_cyclic_dist_father_frame_address,
                                          origin_frame_index,
                                          origin_page_index,
                                          ((cur_page_index << OFFSET_WIDTH) + i));

            if (is_son_empty && (son_frame_index != origin_frame_index)){ // Not to disconnect where we came from
                PMwrite(cur_frame_address + i, 0);
                *zero_table_frame_index = son_frame_index;
            }
        } else {
            num_of_zero_rows++;
        }
    }
    if (num_of_zero_rows == PAGE_SIZE){
        return true; // means we are a table with zeros only.
    }
    return false;
}

void resetFrame(word_t table_frame_ind){
    uint64_t table_frame_address = table_frame_ind * PAGE_SIZE;
    for (int i = 0; i < PAGE_SIZE; ++i){
        PMwrite(table_frame_address + i, 0);
    }
}

uint64_t calc_cyclic_dist(uint64_t page_1, uint64_t page_2){
    uint64_t abs_dist = abs(page_1 - page_2); // Todo: Guy will write a forum Q about it.
    return fmin(NUM_PAGES - abs_dist, abs_dist);
}
