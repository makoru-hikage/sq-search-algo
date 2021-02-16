#include <stdio.h>
#include <math.h>

#define MAX_SIZE 23
#define FOUND 1
#define NOT_YET_FOUND 0

#ifndef NEEDLE
#define NEEDLE 99
#endif //NEEDLE

/**
 * @brief Find the nearest square root (e.g. 23 yields 5 since sqrt(25) is 5)
 * 
 * @param n 
 * @return int 
 */
int find_nearest_sqrt (int n){
    int b = 1;
    while (b*b < n){
        b++;
    }
    return (int)sqrt(b*b);
}

/**
 * @brief Finds the row index of a cell in a given square with base b
 * 
 * @param n cell index
 * @param b the base of the square.
 * @return int 
 */
int row_index(int n, int b){
    return (int)ceil((double)n/b);
}

/**
 * @brief Finds the column index of a cell in a given square with base b
 * 
 * @param n cell index
 * @param b the base of the square.
 * @return int 
 */
int column_index(int n, int b){
    return n - b * (int) ceil ((double)n / b) + b;
}

/**
 * @brief Find the opposite number of an integer in a given base (eg. the opposite of 3 is 2 in the base of 5)
 * 
 * @param n 
 * @param b 
 * @return int 
 */
int diametric_opposite(int n, int b){
    return (b + 1) - n;
}

/**
 * @brief Find the opposite cell of a cell in its row
 * 
 * @param n 
 * @param b 
 * @return int 
 */
int horizontal_opposite (int n, int b){
    //Find the row index
    int r = row_index(n,b);
    //Find the column index
    int c = column_index(n,b);
    // Find the diametric opposite of the column index along the base
    c = diametric_opposite(c,b);

    return c + b*r - b;
}

/**
 * @brief Find the opposite cell of a cell in its column
 * 
 * @param n 
 * @param b 
 * @return int 
 */
int vertical_opposite (int n, int b){
    //Find the row index
    int r = row_index(n,b);
    //Find the column index
    int c = column_index(n,b);
    // Find the diametric opposite of the row index along the base
    r = diametric_opposite(r,b);

    return c + b*r - b;
}

/**
 * @brief Find the opposite cell of a cell in its respectve downright slope
 * 
 * @param n 
 * @param b 
 * @return int 
 */
int descending_opposite (int n, int b){
    //Find the row index
    int r = row_index(n,b);
    //Find the column index
    int c = column_index(n,b);
    // Find the diametric opposite of the row index and column index along the base
    // Then add them
    int intersection_sum = diametric_opposite(r,b) + diametric_opposite(c,b);
    // Row index of the opposite cell
    r = intersection_sum - diametric_opposite(r,b);
    // Column index of the opposite cell
    c = intersection_sum - diametric_opposite(c,b);

    return c + b*r - b;
}

/**
 * @brief Find a number using four indices in one loop iteration
 * 
 * @param haystack 
 * @param size_haystack 
 * @param needle 
 * @return the cell index number where the needle was found, returns 0 if not found
 */
int square_search(int haystack[], int size_haystack, int needle){
    // The square root of the nearest square to the size of the haystack
    int base = find_nearest_sqrt(size_haystack);
    // The floored "median" of the base
    int median_base = (int) floor ((double)(base + 1) / 2);
    // Always start at cell index 1
    int cell = 1;
    // The cell to be lastly iterated
    int last_iterated_cell = median_base + median_base*base - base;

    while (cell <= last_iterated_cell){
        int ho = horizontal_opposite(cell, base);
        int vo = vertical_opposite(cell, base);
        int dso = descending_opposite(cell, base);

        if (haystack[cell-1] == needle){
            return cell;
        }

        if (ho <= size_haystack && haystack[ho-1] == needle){
            return ho;
        }

        if (vo <= size_haystack && haystack[vo-1] == needle){
            return vo;
        }

        if (dso <= size_haystack && haystack[dso-1] == needle){
            return dso;
        }

        if (column_index(cell, base) != median_base){
            cell++;
        } else {
            cell = cell + base - median_base + 1;
        }
    }

    return 0;
}

/**
 * @brief A verbose version of square_search function and stops only when it completely checks all the possible cells
 * 
 * @param haystack 
 * @param size_haystack 
 * @param needle 
 * @return the cell index number where the needle was found, returns 0 if not found 
 */
int square_search_debug(int haystack[], int size_haystack, int needle){
    // The square root of the nearest square to the size of the haystack
    int base = find_nearest_sqrt(size_haystack);
    // The floored "median" of the base
    int median_base = (int) floor ((double)(base + 1) / 2);
    // Always start at cell index 1
    int cell = 1;
    // The cell to be lastly iterated
    int last_iterated_cell = median_base + median_base*base - base;

    //Debugging-related variables
    int needle_status = NOT_YET_FOUND;
    int iteration = 0;
    int found_at_iteration = 0;
    int found_at_cell = 0;

    printf("\n Needle = %d", needle);
    printf("\n Base = %d\n", base);

    while (cell <= last_iterated_cell){
        iteration++;
        int ho = horizontal_opposite(cell, base);
        int vo = vertical_opposite(cell, base);
        int dso = descending_opposite(cell, base);

        int ho_value = ho <= size_haystack ? haystack[ho-1] : 0;
        int vo_value = vo <= size_haystack ? haystack[vo-1] : 0;
        int dso_value = dso <= size_haystack ? haystack[dso-1] : 0;

        printf("\nCurrent cell index and value: %d: %d", cell, haystack[cell-1]);
        printf("\nHorizontal Opposite index and value: %d: %d", ho, ho_value);
        printf("\nVertical Opposite index and value: %d: %d", vo, vo_value);
        printf("\nDescending Opposite index and value: %d: %d\n", dso, dso_value);

        if (haystack[cell-1] == needle){
            needle_status = FOUND;
            found_at_iteration = iteration;
            found_at_cell = cell;
        }

        if (ho <= size_haystack && haystack[ho-1] == needle){
            needle_status = FOUND;
            found_at_iteration = iteration;
            found_at_cell = ho;
        }

        if (vo <= size_haystack && haystack[vo-1] == needle){
            needle_status = FOUND;
            found_at_iteration = iteration;
            found_at_cell = vo;
        }

        if (dso <= size_haystack && haystack[dso-1] == needle){
            needle_status = FOUND;
            found_at_iteration = iteration;
            found_at_cell = dso;
        }

        if (column_index(cell, base) != median_base){
            cell++;
        } else {
            cell = cell + base - median_base + 1;
        }
    }

    printf("\nNeedle: %d", needle);
    printf("\nIterations: %d", iteration);

    if (needle_status == FOUND){
        printf("\nNeedle found on iteration %d", found_at_iteration);
        printf("\nNeedle found on cell %d\n\n", found_at_cell);
        return found_at_cell;
    }

    printf("\nNeedle not found");
    return 0;
}

int main (int argc, char *argv[]){
    int size_haystack = MAX_SIZE;
    int haystack[MAX_SIZE] = {69, 49, 4, 50, 47, 95, 60, 31, 32, 77, 67, 41, 33, 98, 16, 13, 68, 90, 42, 99, 27, 7, 52};
    int needle = NEEDLE;

    printf("\n Haystack = {69, 49, 4, 50, 47, 95, 60, 31, 32, 77, 67, 41, 33, 98, 16, 13, 68, 90, 42, 99, 27, 7, 52}\n");

    int found = square_search(haystack, size_haystack, needle);

    if (found){
        printf("\nThe needle, %d, is found at cell %d (index %d).\n ", needle, found, found - 1);
    }

    return 0;
}