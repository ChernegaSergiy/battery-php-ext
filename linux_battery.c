#define _GNU_SOURCE
#if defined(__linux__) || defined(__ANDROID__)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>

static int read_int_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    int v = -1;
    if (fscanf(f, "%d", &v) != 1) v = -1;
    fclose(f);
    return v;
}

static int read_status_file(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char buf[64] = {0};
    if (fscanf(f, "%63s", buf) != 1) {
        fclose(f);
        return -1;
    }
    fclose(f);
    if (strcasecmp(buf, "Charging") == 0) return 1;
    if (strcasecmp(buf, "Discharging") == 0) return 0;
    if (strcasecmp(buf, "Full") == 0) return 1;
    return -1;
}

static int is_battery_type(const char *base, const char *name)
{
    char path[PATH_MAX];
    char buf[64] = {0};
    snprintf(path, sizeof(path), "%s/%s/type", base, name);
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    if (fscanf(f, "%63s", buf) != 1) {
        fclose(f);
        return 0;
    }
    fclose(f);
    /* Check if type is "Battery" */
    if (strcasecmp(buf, "Battery") == 0) return 1;
    return 0;
}

int linux_battery_level(void)
{
    const char *base = "/sys/class/power_supply";
    DIR *d = opendir(base);
    if (!d) return -1;
    struct dirent *ent;
    char path[PATH_MAX];
    int found = -1;
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;
        
        /* Check type instead of name */
        if (!is_battery_type(base, ent->d_name)) continue;

        snprintf(path, sizeof(path), "%s/%s/capacity", base, ent->d_name);
        found = read_int_file(path);
        if (found >= 0) break;
    }
    closedir(d);
    return found;
}

int linux_battery_is_charging(void)
{
    const char *base = "/sys/class/power_supply";
    DIR *d = opendir(base);
    if (!d) return -1;
    struct dirent *ent;
    char path[PATH_MAX];
    int found = -1;
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;
        
        /* Check type instead of name */
        if (!is_battery_type(base, ent->d_name)) continue;

        snprintf(path, sizeof(path), "%s/%s/status", base, ent->d_name);
        found = read_status_file(path);
        if (found != -1) break;
    }
    closedir(d);
    return found;
}
#else
int linux_battery_level(void) { return -1; }
int linux_battery_is_charging(void) { return -1; }
#endif
