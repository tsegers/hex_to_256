#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* 
 * Table Refernce:
 * https://upload.wikimedia.org/wikipedia/en/1/15/Xterm_256color_chart.svg
 */

/* Holds the color hex vals and an their offset in the table */
int colors[6][2] = {
    { 0, 0x00}, { 1, 0x5f}, { 2, 0x87},
    { 3, 0xaf}, { 4, 0xd7}, { 5, 0xff}
};

/* Holds the greyscale hex vals and an their offset in the table */
int grayscales[24][2] = {
    { 0, 0x08}, { 1, 0x12}, { 2, 0x1c},
    { 3, 0x26}, { 4, 0x30}, { 5, 0x3a},
    { 6, 0x44}, { 7, 0x4e}, { 8, 0x58},
    { 9, 0x62}, {10, 0x6c}, {11, 0x76},
    {12, 0x80}, {13, 0x8a}, {14, 0x94},
    {15, 0x9e}, {16, 0xa8}, {17, 0xb2},
    {18, 0xbc}, {19, 0xc6}, {20, 0xd0},
    {21, 0xda}, {22, 0xe4}, {23, 0xee}
};

/**
 * Converts a hex char [0-F] to its integer val [0-15].
 */
int char_to_hex_val(char c)
{
    if (c - '0' < 10) {
        return c - '0';
    } else {
        return 10 + toupper(c) - 'A';
    }
}

/**
 * Determines the nearest 256 color by combining the nearest hex color for each part
 * of the RGB triplet and computing the 256 color.
 *
 * @param color pointer to an array of chars holding the color int he form ABCDEF
 * @param result pointer to value in which to puth the 0-255 result
 * @param error pointer to value in which to put the error margin
 */
void nearest_color(char *color, int *result, int *error)
{
    int color_error = 0;
    int index_array[3] = {0};

    /*
     * Loops three times (once for each element \in {R, G, B}) and finds the
     * index of the nearest color in the colors array.
     */
    for (int c = 0; c < 3; c++) {
        int current_pair_val = 16 * (char_to_hex_val(color[2 * c])) +
                               1 * (char_to_hex_val(color[2 * c + 1]));
        int lowest_pair_error = 0xFF;

        /* Iterate over the colors array in search of the nearest value */
        for (int i = 0; i < 6; i++) {
            int error = 0;

            /* If a value with a lower error is found, update the lowest value and the index */
            if ((error = abs(current_pair_val - colors[i][1])) < lowest_pair_error) {
                index_array[c] = colors[i][0];
                lowest_pair_error = error;
            }
        }

        color_error += lowest_pair_error;
    }

    /* Pass back the results and return */
    *result = (16 + 36 * index_array[0] + 6 * index_array[1] + index_array[2]);
    *error = color_error;
}

/**
 * Determines the nearest 256 grayscale by computing the hex grayscale with the
 * lowest error.
 *
 * @param color pointer to an array of chars holding the color int he form ABCDEF
 * @param result pointer to value in which to puth the 0-255 result
 * @param error pointer to value in which to put the error margin
 */
void nearest_grayscale(char *color, int *result, int *error)
{
    int lowest_gray_error = 0xFFFFFF;
    int best_index;

    /* 
     * Iterate over the grayscales array in search of the value with the lowest
     * error.
     */
    for (int g = 0; g < 24; g++) {
        int current_gray_error = 0;

        /* 
         * Determine the error by adding the individual errors for each element
         * \in {B, G, B} 
         */
        for (int c = 0; c < 3; c++) {
            int current_pair_val = 16 * (char_to_hex_val(color[2 * c])) +
                                   1 * (char_to_hex_val(color[2 * c + 1]));
            current_gray_error += abs(current_pair_val - grayscales[g][1]);
        }

        /* If a value with a lower error is found, update the lowest value and the index */
        if (current_gray_error < lowest_gray_error) {
            lowest_gray_error = current_gray_error;
            best_index = grayscales[g][0];
        }
    }

    /* Pass back the results and return */
    *error = lowest_gray_error;
    *result = 232 + best_index;
}

/**
 * Converts a string in the form XXXXX to the 256 color with the nearest value.
 *
 * @param color pointer to an array of chars holding the color int he form ABCDEF
 */
int hex_to_256(char *color)
{
    int color_result;
    int color_error;
    int gray_result;
    int gray_error;

    /* Check if input is valid */
    for (int i = 0; i < 6; i++) {
        if (!isxdigit(toupper(color[i]))) {
            return -1;
        }
    }

    nearest_grayscale(color, &color_result, &color_error);
    nearest_color(color, &gray_result, &gray_error);

    /* Decide whether to go for a grayscale or for a color */
    if (color_error < gray_error) {
        return color_result;
    } else {
        return gray_result;
    }
}

/**
 * Main function of the program.
 *
 * @param argc the number of command-line arguments the program was invoked
 *     with.
 * @param argv pointer to an array of character strings that contain the
 *     arguments.
 *
 * @returns integer denoting the status of the program at termination.
 *     0 means normal termination
 */
int main(int argc, const char *argv[])
{
    /* Temp variables */
    const char *color_in;
    int color_256;

    /*
     * Process the arguments, pass them trough the converter and print the
     * output
     */
    for (int i = 1; i < argc; i++) {
        color_in = argv[i];
        color_256 = hex_to_256((char *) color_in);

        printf("Hex: #%s \n256: %7.0i\n\n", color_in, color_256);
    }

    /* Exit */
    return 0;
}

