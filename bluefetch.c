#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RESET "\033[0m"
#define BOLD "\033[1m"

// Dynamic Colors based on terminal theme
#define COLOR1 "\033[38;5;2m"
#define COLOR2 "\033[38;5;4m"
#define COLOR3 "\033[38;5;6m"

// Function to execute a command and get the output
int exec_cmd(const char *cmd, char *result, size_t result_size) {
    FILE *fp;
    fp = popen(cmd, "r");
    if (fp == NULL) return 0;
    if (fgets(result, result_size, fp) == NULL) {
        pclose(fp);
        return 0;
    }
    pclose(fp);
    result[strcspn(result, "\n")] = 0; // Remove newline
    return 1;
}

int main() {
    char user[128], hostname[128], os[128], kernel[128], uptime[128];
    char shell[128], resolution[128], memory[128], cpu[256], gpu[256];

    // Fetching system information with error checking
    int has_user = exec_cmd("whoami", user, sizeof(user));
    int has_hostname = exec_cmd("hostname", hostname, sizeof(hostname));
    int has_os = exec_cmd("grep '^PRETTY_NAME=' /etc/os-release | cut -d '\"' -f 2", os, sizeof(os));
    int has_kernel = exec_cmd("uname -r", kernel, sizeof(kernel));
    int has_uptime = exec_cmd("uptime -p", uptime, sizeof(uptime));
    int has_shell = exec_cmd("basename \"$SHELL\"", shell, sizeof(shell));
    int has_resolution = exec_cmd("xdpyinfo | grep 'dimensions:' | awk '{print $2}'", resolution, sizeof(resolution));
    int has_memory = exec_cmd("free -h | awk '/^Mem:/ {print $3 \"/\" $2}'", memory, sizeof(memory));
    int has_cpu = exec_cmd("grep -m1 'model name' /proc/cpuinfo | cut -d ':' -f2-", cpu, sizeof(cpu));
    int has_gpu = exec_cmd("lspci | grep -i vga | cut -d ':' -f3-", gpu, sizeof(gpu));

    // Displaying the fetched information with ASCII art on the left
    printf(COLOR1 "╔══════════════════════════════════╗" RESET "   " COLOR3 "User:       " RESET "%s@%s\n", user, hostname);
    printf(COLOR1 "║██████╗ ██╗     ██╗   ██╗███████╗ ║" RESET "   " COLOR3 "OS:         " RESET "%s\n", os);
    printf(COLOR1 "║██╔══██╗██║     ██║   ██║██╔════╝ ║" RESET "   " COLOR3 "Kernel:     " RESET "%s\n", kernel);
    printf(COLOR1 "║██████╔╝██║     ██║   ██║█████╗   ║" RESET "   " COLOR3 "Uptime:     " RESET "%s\n", uptime);
    printf(COLOR1 "║██╔══██╗██║     ██║   ██║██╔══╝   ║" RESET "   " COLOR3 "Shell:      " RESET "%s\n", shell);
    printf(COLOR1 "║██████╔╝███████╗╚██████╔╝███████╗ ║" RESET "   " COLOR3 "Resolution: " RESET "%s\n", resolution);
    printf(COLOR1 "╚═════╝ ╚══════╝ ╚═════╝ ╚══════╝  ╚═" RESET "   " COLOR3 "Memory:     " RESET "%s\n", memory);
    printf(COLOR1 "╚══════════════════════════════════╝" RESET "   " COLOR3 "CPU:        " RESET "%s\n", cpu);
    if (has_gpu) printf("                                   " COLOR3 "GPU:        " RESET "%s\n", gpu);

    printf(COLOR2 "════════════════════════════════════" RESET "   " COLOR3 "I need sleep\n" RESET);

    return 0;
}
