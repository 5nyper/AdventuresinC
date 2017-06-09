#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <string.h>
#include <stdarg.h>

int sock;
char server_reply[512];


#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <string.h>
#include <stdarg.h>

int sock;
char server_reply[512];

int readUntilNewline() // credits to sniger for bug catch
{
  int i;
  for(i = 0; i < 511; ++i){
      char rec;
      read(sock, &rec, 1);
      if(rec == '\n'){
         server_reply[i] = '\0';
         return 1;
         }
      else server_reply[i] = rec;
    }
  server_reply[511] = '\0';
  return 1;
}
int indexof(char* str, char val) {
  int i;
  for(i = 0; i<strlen(str); i++) {
    if(str[i] == val) {
      return i;
    }
  }
  return -1;
}

int main() {
  int read_bytes;
  struct sockaddr_in server;
  char copied_message[513];
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    puts("failure");
  }
  server.sin_addr.s_addr = inet_addr("178.32.215.69");
  server.sin_family = AF_INET;
  server.sin_port = htons(6667);
  if(connect(sock , &server , sizeof(server)) < 0) {
    puts("Failed to connect to IRC");
  }
  puts("connectd");
  char nick[] = "NICK danC\r\n";
  char indent[] = "USER danC 0 0 :danC\r\n";
  if(send(sock, nick, strlen(nick), 0) < 0)
    puts("failed to write NICK info");
  if(send(sock, indent, strlen(indent), 0) < 0)
    puts("failed to write INDENT info");
    int joined = 0;
   while (readUntilNewline() && strncpy(copied_message, server_reply, 512)) {
        printf(">> %s\n", copied_message);
        if(strncmp(copied_message, "PING", 4) == 0) {
          copied_message[1] = 'O';
          send(sock, copied_message, strlen(copied_message), 0);
        }
        else if(strncmp(copied_message, ":", 1) == 0 && !joined) {
          char* split;
          int wordcount = 0;
          split = strtok(copied_message, " ");
          while(split != NULL) {
            if(wordcount == 1 && strncmp(split, "001", 3) == 0) {
              char join[] = "JOIN #insecurity\r\n";
              if(send(sock, join, strlen(join), 0) < 0)
                puts("failed to JOIN");
              joined = 1;
             // printf("%s\n", split);
            }
            split = strtok(NULL, " ");
            wordcount++;
          }
        }
        else if(strncmp(copied_message, ":", 1) == 0 && joined == 1) {
          char user[10];
          char* split;
          int wordcount = 0;
          split = strtok(copied_message, " ");
          char msg[1000] = "";
          while(split != NULL) {
            if(wordcount == 0) {
              if(indexof(split, '!') > 0) {
                int index = indexof(split, '!');
                strncpy(user, split, index);
                user[index + 1] = '\0';
                printf("%s ", user);
              }
            }
            else if(wordcount == 1) {
              if(strncmp(split, "PRIVMSG", 7) == 0) {
                printf("says");
                split = strtok(NULL, " ");
                printf(" in %s ", split);
              }
            }
            else {
              strcat(msg, " ");
              strcat(msg, split);
            }
            split = strtok(NULL, " ");
            wordcount++;
          }
          wordcount = 0;
          char* cmd_split;
          int in_cmd = 0;
          char *args = (char *)malloc(100);
          //strcpy(args, "");
          cmd_split = strtok(msg, " ");
          while(cmd_split != NULL) {
            if(wordcount == 0) {
              if(strncmp(cmd_split, ":!say", 4) == 0) {
                puts("first cmd is !say");
                in_cmd = 1;
              }
              else if(strncmp(cmd_split, ":!kill", 4) == 0 && strncmp(user, ":vikat0n", 8) == 0) {
                close(sock);
              }
            }
            else if(in_cmd == 1) {
              strcat(args, cmd_split);
              strcat(args, " ");
            }
           // printf("%s\n", cmd_split);
            cmd_split = strtok(NULL, " ");
            wordcount++;
          }
          if(in_cmd) {
            puts(args);
            char send_msg[100] = "PRIVMSG  #insecurity :";
            strcat(send_msg, args);
            strcat(send_msg, "\r\n");
            puts(send_msg);
            if(write(sock, send_msg, strlen(send_msg)) < 0)
              puts("failed to PRV+IVMGSD");
            free(args);
          }
        }
        else if(strncmp(server_reply, "ERROR", 5) == 0) {
          close(sock);
          puts("Connection closed");
          break;
        }
      }
  return 0;
}
