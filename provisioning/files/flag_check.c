/*
 * flag_check.c  –  Dirty COW Lab Task-1 flag checker
 *
 * This binary runs as root (setuid 4755).
 * It verifies that the student has successfully exploited the Dirty COW
 * race condition by checking whether /zzz contains the expected modified
 * string "111111******333333".
 *
 * If verified: prints the content of /root/protected_dirtycow1.txt (the flag).
 * Otherwise:   prints an error message.
 *
 * Build & install (done by Ansible):
 *   gcc -o /usr/local/bin/flag_check flag_check.c
 *   chown root:root /usr/local/bin/flag_check
 *   chmod 4755 /usr/local/bin/flag_check
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define ZZZ_PATH        "/zzz"
#define FLAG1_PATH      "/root/protected_dirtycow1.txt"
#define EXPECTED_STR    "111111******333333"
#define BUFSIZE         256

int main(void)
{
    /* Drop any supplementary groups, keep effective UID=0 from setuid bit */
    if (setgroups(0, NULL) != 0 && errno != EPERM) {
        /* non-fatal on some kernels */
    }

    /* ── Read /zzz ─────────────────────────────────────────────────────── */
    FILE *fzzz = fopen(ZZZ_PATH, "r");
    if (!fzzz) {
        fprintf(stderr, "[flag_check] Cannot open %s: %s\n", ZZZ_PATH, strerror(errno));
        return 1;
    }

    char zzz_content[BUFSIZE];
    memset(zzz_content, 0, sizeof(zzz_content));
    if (fgets(zzz_content, sizeof(zzz_content), fzzz) == NULL) {
        fclose(fzzz);
        fprintf(stderr, "[flag_check] Failed to read %s\n", ZZZ_PATH);
        return 1;
    }
    fclose(fzzz);

    /* Strip trailing newline */
    size_t len = strlen(zzz_content);
    if (len > 0 && zzz_content[len - 1] == '\n') {
        zzz_content[len - 1] = '\0';
    }

    /* ── Check expected modification ───────────────────────────────────── */
    if (strcmp(zzz_content, EXPECTED_STR) != 0) {
        printf("[flag_check] Task not completed yet.\n");
        printf("             /zzz currently contains: \"%s\"\n", zzz_content);
        printf("             Expected:                 \"%s\"\n", EXPECTED_STR);
        printf("\nHint: Run ./cow_attack in /home/seed/Labsetup/, wait a few seconds, then try again.\n");
        return 2;
    }

    /* ── Task verified – print the flag ────────────────────────────────── */
    printf("[flag_check] Task 2 verified! Here is your flag:\n\n");

    FILE *fflag = fopen(FLAG1_PATH, "r");
    if (!fflag) {
        fprintf(stderr, "[flag_check] Cannot open flag file: %s\n", strerror(errno));
        return 1;
    }

    char line[BUFSIZE];
    while (fgets(line, sizeof(line), fflag) != NULL) {
        fputs(line, stdout);
    }
    fclose(fflag);

    printf("\n");
    return 0;
}
