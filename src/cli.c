/// @author : github.com/saber-88 @contributes: wallrift-daemon
/// @author : github.com/its-19818942118 @contributes: wallrift-cli

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define SOCK_PATH "/tmp/wallrift.sock"

enum State {
    eTrue ,
    eError ,
    eFalse ,
};

void printHelp(){
  printf("\033[1mwallrift - A smooth parllax supported wallpaper engine\033[0m.\n\n");
  printf("\033[1m\033[4mUsage\033[0m: \033[1mwallrift <COMMAND>\033[0m\n\n");
  printf("\033[1m\033[04mCommands:\033[0m \n\n");
  printf("  \033[1mimg\033[0m      sends the path of the wallpaper to the daemon.\n");
  printf("  \033[1mspeed\033[0m    sets the speed for parllax. Lower speed = smooth interpolation.\n");

  printf("\033[1m\033[04mOptions:\033[0m \n\n");
  printf("  \033[1m-h,--help\033[0m     prints this help message.\n");
}

int handleSocket(char const* restrict wallpath , float const speed, enum State stateImgF , enum State stateSpeedF) {
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock_fd == -1) {
      perror("Failed to open socket\n");
      return EXIT_FAILURE;
    }
  
    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) - 1); 
  
    if (connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
      perror("Connection to socket failed!\n");
      return EXIT_FAILURE;
    }
  
    char cmd[1024];
    
    // commented out cursed code of shame
    // snprintf(cmd, sizeof(cmd), "img %s speed %f",wallpath,speed);
    
    if (stateSpeedF == eTrue) {
        snprintf(cmd, sizeof(cmd), "speed %f", speed);
    }
    else if ( stateImgF == eTrue ) {
        snprintf(cmd, sizeof(cmd), "img %s", wallpath);
    }
    
    else {
        snprintf(cmd, sizeof(cmd), "img %s speed %f", wallpath , speed);
    }
    
    if (write(sock_fd, cmd, strlen(cmd)) == -1) {
      perror("write to socket failed!\n");
      return EXIT_FAILURE;
    }
  
    close(sock_fd);
    return EXIT_SUCCESS;
    
}

int validateFloat(char const* restrict string, float* outValue) {
    if (string == NULL || *string == '\0') {
        return EXIT_FAILURE;
    }

    char* endptr;
    errno = 0; // Reset errno to catch out-of-range errors
    
    float val = strtof(string, &endptr);

    // 1. Check if any conversion happened (string != endptr)
    // 2. Check if we reached the end of the string (*endptr == '\0')
    // 3. Check for overflow/underflow (errno == 0)
    if (string != endptr && *endptr == '\0' && errno == 0) {
        if (outValue) {
            *outValue = val;
        }
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

int validateImgPath(char const* restrict imgPath) {
    struct stat fileStatus;
    
    if (stat(imgPath, &fileStatus) != 0) {
        return EXIT_FAILURE;
    }
    
    if (!S_ISREG(fileStatus.st_mode)) {
        return EXIT_FAILURE;
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printHelp();
        return EXIT_FAILURE;
    }
    
    float speed = {0.5f};
    char const* wallpath = {NULL};
    
    enum State stateImgF = eFalse;
    enum State stateSpeedF = eFalse;
    enum State stateQueryF = eFalse;
    
    for (size_t argsItr = {1}; argsItr < argc; ++argsItr) {
        #define TEST_NEXT_ARGUMENT argc > ( argsItr + 1 ) // tests for more arguments
        #define CMP_ARGUMENT(...) strcmp(argument, __VA_ARGS__) == EXIT_SUCCESS
        
        char* argument = argv[argsItr];
        
        if (CMP_ARGUMENT("-h") || CMP_ARGUMENT("--help")) {
            printHelp();
            return EXIT_SUCCESS;
        }
        
        else if (CMP_ARGUMENT("img")) {
            if (stateImgF == eTrue) {
                printf("ignoring duplicate request: 'img'\n");
            }
            else if (TEST_NEXT_ARGUMENT && validateImgPath(argv[++argsItr]) == EXIT_SUCCESS) {
                stateImgF = eTrue;
                wallpath = argv[argsItr];
                printf("received request: 'img'\n");
            }
            else {
                stateImgF = eError;
                printf("invalid argument for 'img'. expected valid path!\n");
            }
        }
        
        else if (CMP_ARGUMENT("speed")) {
            if (stateSpeedF == eTrue) {
                printf("ignoring duplicate request: 'speed'\n");
            }
            else if (TEST_NEXT_ARGUMENT && validateFloat(argv[++argsItr], &speed) == EXIT_SUCCESS) {
                stateSpeedF = eTrue;
                printf("received request: 'speed' (%.2f)\n", speed);
            }
            else {
                stateSpeedF = eError;
                printf("invalid argument for 'speed'. expected a float!\n");
            }
        }
        
        else if (CMP_ARGUMENT("query")) {
            if (stateQueryF == eTrue) {
                printf("ignoring duplicate request: 'query'\n");
            }
            else {
                stateQueryF = eTrue;
                printf("recived request: 'query'\n");
            }
        }
        
        else {
            printf("invalid request recived: '%s'. please provide a valid request! try -h or --help for information\n" , argument);
            return EXIT_FAILURE;
        }
    }
    
    if ( stateImgF == eError || stateSpeedF == eError ) {
        printf("Exiting due to errors!");
        return EXIT_FAILURE;
    }
    
    handleSocket(wallpath, speed , stateImgF , stateSpeedF);
    
}
