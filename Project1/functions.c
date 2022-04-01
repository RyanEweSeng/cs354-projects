/******************************************************************************
 * @file: functions.c
 *
 * retoh
 * Ryan Toh
 * 908 160 7708
 * OTHER COMMENTS FOR THE GRADER (OPTIONAL)
 *
 * @creator: Ryan Toh (retoh@wisc.edu)
 * @modified: SUBMISSION DATE
 *****************************************************************************/
#include <stdio.h>
#include "functions.h"

// Some macros that may be useful to you 
#define MAX_USERNAME_LEN    32
#define MAX_EMAIL_LEN       32
#define MAX_DOMAIN_LEN      64
#define MIN_PASSWORD_LEN    8
#define MAX_PASSWORD_LEN    16
#define NEW_LINE_CHAR       10

// Set this to 1 to enable dbgprintf statements, make sure to set it back to 0 
// before submitting!
#define DEBUG               0 
#define dbgprintf(...)      if (DEBUG) { printf(__VA_ARGS__); }

/******************************************************************************
 * Helper functions
 *****************************************************************************/

int get_len(char str[]) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        count++;
    }

    return count;
}

int substr_length(char *start, char *end) {
    int len = 0;
    for (char *p = start; p < end; p++) { len++; }

    return len;
}

int domain_length(char *start, char *end) {
    int count = 0;
    for (char *p = start; p < end; p++) {
        if (*p != '.') { count++; }
    }

    return count;
}

int is_dotcom(char *ptr) {
    char d1[] = ".com";
    char *p_d1 = &d1[0];
    while (*p_d1 != '\0') {
        if (*ptr != *p_d1) { return 0; }
        p_d1++;
        ptr++;
    }

    return 1;
}

int is_dotedu(char *ptr) {
    char d2[] = ".edu";
    char *p_d2 = &d2[0];
    while (*p_d2 != '\0') {
        if (*ptr != *p_d2) { return 0; }
        p_d2++;
        ptr++;
    }

    return 1;
}

int is_dotnet(char *ptr) {
    char d3[] = ".com";
    char *p_d3 = &d3[0];
    while (*p_d3 != '\0') {
        if (*ptr != *p_d3) { return 0; }
        p_d3++;
        ptr++;
    }

    return 1;
}

int valid_top_domain(char *ptr) {
    if (is_dotcom(ptr) || is_dotedu(ptr) || is_dotnet(ptr)) { return 1; }
    else { return 0; }
}

int is_alpha(char c) {
    int ascii_value = (int) c;
    if ((65 <= ascii_value && ascii_value <= 90) || (97 <= ascii_value && ascii_value <= 122)) { return 1; }
    else { return 0; }
}

int is_upper(char c) {
    int ascii_value = (int) c;
    if (65 <= ascii_value && ascii_value <= 90) { return 1; }
    else { return 0; }
}

int is_lower(char c) {
    int ascii_value = (int) c;
    if (97 <= ascii_value && ascii_value <= 122) { return 1; }
    else { return 0; }
}

int is_digit(char c) {
    int ascii_value = (int) c;
    if (48 <= ascii_value && ascii_value <= 57) { return 1; }
    else { return 0; }
}

int is_underscore(char c) {
    int ascii_value = (int) c;
    if (ascii_value == 95) { return 1; }
    else { return 0; }
}

int is_space(char c) {
    int ascii_value = (int) c;
    if (ascii_value == 32) { return 1; }
    else { return 0; }
}

/******************************************************************************
 * Verification functions
 *****************************************************************************/

/*
 * A username must begin with a letter [A-Z, a-z], contain 32 characters
 * or less, and  may only consist of letters, underscores, or 
 * digits [A-Z, a-z, _, 0-9]. An error message is displayed if any of 
 * these conditions are not met. Only print the first applicable error 
 * message. 
 *
 * @param user : The username string
 * @param len : Size of the username input buffer
 * @return 1 if valid, 0 if not
 */
int Verify_Username(char user[], size_t len) {

    /* BEGIN MODIFYING CODE HERE */
    dbgprintf("DEBUG PRINT MODE ON\n");

    if (!is_alpha(user[0])) {
        printf(ERROR_01_USER_START_INVALID);
        return 0;
    } else if (get_len(user) > MAX_USERNAME_LEN) {
        printf(ERROR_02_USER_LEN_INVALID);
        return 0;
    } else {
        for (int i = 0; user[i] != '\0'; i++) {
            if (!is_alpha(user[i]) && !is_digit(user[i]) && !is_underscore(user[i])) {
                printf(ERROR_03_USER_CHARS_INVALID);
                return 0;
            }
        }
    }
    /* END MODIFYING CODE HERE */

    printf(SUCCESS_1_USER);
    return 1;
}

/*
 * An email address has four parts:
 *      name
 *          exists
 *          must start with letter
 *          max 32 characters
 *          may contain only letters and digits
 *      @ symbol
 *          exists
 *      domain name
 *          exists
 *          max of 64 characters
 *          composed of one or more subdomains separated by .
 *          subdomain must begin with a letter
 *          subdomains may contain only letters and digits
 *      top-level domain 
 *          must be [.edu, .com, .net]
 *
 * If the email address contains one or more errors print only the first
 * applicable error from the list.
 *
 * Note this task is based on a real world problem and may not be the best 
 * order to approach writing the code.
 *
 * @param email : The email string
 * @param len : Size of the email input buffer
 * @return 1 if valid, 0 if not
 */
int Verify_Email(char email[], size_t len) {

    /* BEGIN MODIFYING CODE HERE */
    char *at_symbol_ptr = &email[0];
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_symbol_ptr = &email[i]; 
            break;
        }
    }

    char *end_ptr;
    for (int i = 0; i < len; i++) {
        if (email[i] == '\0') {
            end_ptr = &email[i];
            break;
        }
    }

    // no '@' symbol so end of name is at the top-level-domain
    char *top_domain_ptr = end_ptr - 4;
    if (*at_symbol_ptr != '@') {
        if (valid_top_domain(top_domain_ptr)) {
            at_symbol_ptr = top_domain_ptr;
        }
        at_symbol_ptr = end_ptr;
    }

    if (email[0] == '@') {
        printf(ERROR_04_EMAIL_MISSING_NAME);  // example @domain.com
        return 0;
    }

    if (!is_alpha(email[0])) {
        printf(ERROR_05_EMAIL_START_INVALID);
        return 0;
    }

    int email_name_length = substr_length(email, at_symbol_ptr);
    if (email_name_length > MAX_EMAIL_LEN) {
        printf(ERROR_06_EMAIL_NAME_LEN_INVALID);
        return 0;
    }

    for (char *curr = &email[0]; curr < at_symbol_ptr; curr++) {
        if (!is_alpha(*curr) && !is_digit(*curr)) {
            printf(ERROR_07_EMAIL_NAME_CHARS_INVALID);
            return 0;
        }
    }

    if (*at_symbol_ptr != '@') {
        printf(ERROR_08_EMAIL_MISSING_SYMBOL); // example mike.wisc.edu
        return 0;
    }

    if (at_symbol_ptr + 1 == top_domain_ptr) {
        printf(ERROR_09_EMAIL_MISSING_DOMAIN); // example mike@.edu
        return 0;
    }

    int domain_len = domain_length(at_symbol_ptr + 1, top_domain_ptr);
    if (domain_len > MAX_DOMAIN_LEN) {
        printf(ERROR_10_EMAIL_DOMAIN_LEN_INVALID);
        return 0;
    }

    char *domain_ptr = at_symbol_ptr + 1;
    if (!is_alpha(*domain_ptr)) { // check if domain starts with letter
        printf(ERROR_11_EMAIL_DOMAIN_START_INVALID);
        return 0;
    }
    for (char *curr = domain_ptr; curr < top_domain_ptr; curr++) { //check if subdomains start with letter
        char *next = curr + 1;
        if (*curr == '.' && !is_alpha(*next)) {
            printf(ERROR_11_EMAIL_DOMAIN_START_INVALID);
            return 0;
        }
    }

    for (char *curr = domain_ptr; curr < top_domain_ptr; curr++) {
        if (*curr != '.' && !is_alpha(*curr)) {
            printf(ERROR_12_EMAIL_DOMAIN_CHARS_INVALID);
            return 0;
        }
    }

    if (!valid_top_domain(top_domain_ptr)) {
        printf(ERROR_13_TOP_LEVEL_DOMAIN_INVALID);
        return 0;
    }
    /* END MODIFYING CODE HERE */
    
    printf(SUCCESS_2_EMAIL);
    return 1;
}

/*
 * The following password requirements must be verified:
 *  - May use any character except spaces (i.e., "my password" is invalid)
 *  - Must contain at least 8 characters (i.e., "Password" has 8 characters 
 *    and is valid)
 *  - May have at most 16 characters (i.e., "1234567890Abcdef" has 16 
 *    characters and is valid)
 *  - Must contain at least one upper case character [A-Z]
 *  - Must contain at least one lower case character [a-z]
 *
 * @param pwd : The original password string
 * @param len : Size of the original password input buffer
 * @return 1 if valid, 0 if not
 */
int Verify_Password(char pwd[], size_t len) {

    /* BEGIN MODIFYING CODE HERE */
    for (int i = 0; pwd[i] != '\0'; i++) {
        if (is_space(pwd[i])) {
            printf(ERROR_14_PWD_SPACES_INVALID);
            return 0;
        }
    }

    int pwd_len = get_len(pwd);
    if (pwd_len < MIN_PASSWORD_LEN) {
        printf(ERROR_15_PWD_MIN_LEN_INVALID);
        return 0;
    } else if (pwd_len > MAX_PASSWORD_LEN) {
        printf(ERROR_16_PWD_MAX_LEN_INVALID);
        return 0;
    }

    int upper_count = 0;
    int lower_count = 0;
    for (int i = 0; pwd[i] != '\0'; i++) {
        if (is_upper(pwd[i])) { upper_count++; }
        if (is_lower(pwd[i])) { lower_count++; }
    }

    if (upper_count < 1) {
        printf(ERROR_17_PWD_MIN_UPPER_INVALID);
        return 0;
    }

    if (lower_count < 1) {
        printf(ERROR_18_PWD_MIN_LOWER_INVALID);
        return 0;
    }
    /* END MODIFYING CODE HERE */

    return 1;
}
/*
 * Original Password and the Reentered Password must match
 *
 * @param pwd1 : The original password string
 * @param len1 : Size of the original password input buffer
 * @param pwd2 : The reentered password string
 * @param len2 : Size of the renetered password input buffer
 * @return 1 if valid, 0 if not
 */
int Verify_Passwords_Match(char pwd1[], size_t len1, char pwd2[], size_t len2) {

    /* BEGIN MODIFYING CODE HERE */
    char *pwd1_ptr = &pwd1[0];
    char *pwd2_ptr = &pwd2[0];

    while (*pwd1_ptr != '\0' || *pwd2_ptr != '\0') {
        if (*pwd1_ptr == *pwd2_ptr) {
            pwd1_ptr++;
            pwd2_ptr++;
        } else {
            printf(ERROR_19_PWD_MATCH_INVALID);
            return 0;
        }
    }
    /* END MODIFYING CODE HERE */

    printf(SUCCESS_3_PASSWORDS);
    return 1;
}

/******************************************************************************
 * I/O functions
 *****************************************************************************/

/*
 * Prompts user with an input and reads response from stdin
 *
 * @param message : Prompt displayed to the user
 * @param data : char array to hold user repsonse
 * @param MAX_LENGTH : Size of user response input buffer
 */
void Get_User_Data(char *message, char *data, const int MAX_LENGTH) {
    printf("%s", message);
    fgets(data, MAX_LENGTH, stdin);
    /* BEGIN MODIFYING CODE HERE */
    data[get_len(data) - 1] = '\0'; // removes trailing newline character
    /* END MODIFYING CODE HERE */
    return;
}
