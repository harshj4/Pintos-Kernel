#include "userprog/syscall.h"
#include "userprog/process.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "pagedir.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/input.h"
#include "devices/shutdown.h"
#include <string.h>

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  void * esp = f->esp;
  // printf ("%x %x\n", esp, (void (*) (void)) esp);
  // printf ("%p %p %p\n", esp, (esp+4), (esp+8));
  // printf ("%x : %x %x\n", esp, ((char *)f->esp), *((char *)f->esp));
  struct thread * cur = thread_current();
  char ** filename, ** buffer;
  int * file_desc;

  if(!is_user_vaddr (esp) || esp >= 0xbffffffc || esp < 0x08048000) {
    cur->exit_status = -1; thread_exit();
  }

  void * page = pagedir_get_page(cur->pagedir, f->esp);

  if(page == NULL) {
    cur->exit_status = -1; thread_exit();
  }

  // if(esp >= 0xbffffffc || esp < 0x08048000 || page == NULL) {
  //   thread_current()->exit_status = -1; thread_exit();  
  // }

  switch (*((char *)esp))
  {
  /*----------------------------------------------------------*/
  /*                          HALT                            */
  /*----------------------------------------------------------*/
  case SYS_HALT: // args: 0
    // TODO: Need to implement halting properly
    // printf("SYS_HALT\n");
    shutdown_power_off();
    break;
  
  /*----------------------------------------------------------*/
  /*                          Exit                            */
  /*----------------------------------------------------------*/
  case SYS_EXIT: ; // args: 1
    // int8_t status = (esp + 4) >= 0xbffffffc ? -1 : *((int8_t *)(esp + 4));
    // thread_current()->exit_status = status;
    cur->exit_status = *((int8_t *) pagedir_get_page(cur->pagedir, esp+4));
    thread_exit();
    // printf("\n\nSYS_EXIT: %d\n\n", thread_current()->exit_status);
    break;
  
  /*----------------------------------------------------------*/
  /*                          Exec                            */
  /*----------------------------------------------------------*/
  case SYS_EXEC: ;// args: 1
    // printf("SYS_EXEC\n");

    filename = pagedir_get_page(cur->pagedir, esp+4);
    if (*filename == NULL || pagedir_get_page(cur->pagedir, *filename) == NULL) {
      // cur->exit_status = -1;
      // thread_exit();
      f->eax = -1;
    }
    else f->eax = process_execute(*filename);
    break;
  
  /*----------------------------------------------------------*/
  /*                          Wait                            */
  /*----------------------------------------------------------*/
  case SYS_WAIT: ;// args: 1
    // printf("SYS_WAIT\n");
    tid_t * child = pagedir_get_page(cur->pagedir, esp+4);
    f->eax = process_wait(*child);
    break;
  
  /*----------------------------------------------------------*/
  /*                         Create                           */
  /*----------------------------------------------------------*/
  case SYS_CREATE: ;// args: 2
    // printf("SYS_CREATE\n");
    
    // char ** new_file = pagedir_get_page(cur->pagedir, esp+4);      // arg0
    filename = pagedir_get_page(cur->pagedir, esp+4);                 // arg0
    unsigned * init_size = pagedir_get_page(cur->pagedir, esp+8);     // arg1
    if (*filename == NULL || init_size == NULL || pagedir_get_page(cur->pagedir, *filename) == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    // f->eax = (uint32_t) filesys_create(*filename, *init_size);
    f->eax = filesys_create(*filename, *init_size);

    break;
  
  /*----------------------------------------------------------*/
  /*                         Remove                           */
  /*----------------------------------------------------------*/
  case SYS_REMOVE: ;// args: 1
    // printf("SYS_REMOVE\n");
    // char ** old_file = pagedir_get_page(cur->pagedir, esp+4);      // arg0
    filename = pagedir_get_page(cur->pagedir, esp+4);                 // arg0
    if (*filename == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    f->eax = (uint32_t) filesys_remove(*filename);

    break;
  
  /*----------------------------------------------------------*/
  /*                          Open                            */
  /*----------------------------------------------------------*/
  case SYS_OPEN: // args: 1
    // printf("SYS_OPEN\n");
    filename = pagedir_get_page(cur->pagedir, esp+4);                 // arg0
    if (*filename == NULL || pagedir_get_page(cur->pagedir, *filename) == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }

    for(int i = 2; i < MAX_FD; i++) {
      if(cur->fdt[i] == NULL) {
        cur->fdt[i] = filesys_open(*filename);
        if (cur->fdt[i] == NULL) {
          // cur->exit_status = -1;
          // thread_exit();
          f->eax = -1;
          break;              // not reachable
        }
        else {
          f->eax = i;
          break;
        }
      }
    }
    
    break;
  
  /*----------------------------------------------------------*/
  /*                        Filesize                          */
  /*----------------------------------------------------------*/
  case SYS_FILESIZE: // args: 1
    // printf("SYS_FILESIZE\n");
    file_desc = pagedir_get_page(cur->pagedir, esp+4);                // arg0
    if (file_desc == NULL || cur->fdt[*file_desc] == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }

    f->eax = file_length(cur->fdt[*file_desc]);
    break;
  
  /*----------------------------------------------------------*/
  /*                          Read                            */
  /*----------------------------------------------------------*/
  case SYS_READ: // args: 3
    // printf("SYS_READ\n");
    file_desc = pagedir_get_page(cur->pagedir, esp+4);              // arg0
    buffer = pagedir_get_page(cur->pagedir, esp+8);                 // arg1
    int32_t * read_size = pagedir_get_page(cur->pagedir, esp+12);   // arg2

    if (*file_desc >= MAX_FD || *file_desc < 0 || buffer == NULL || *buffer == NULL || *buffer >= PHYS_BASE) {
      cur->exit_status = -1;
      thread_exit();
    }
    else if(*file_desc == 0) {// read
      // printf("%s", (char *)(*buffer));
      // getc(*buffer);
      int ch = 0;
      // *buffer = input_getc();
      while((*(*buffer + ch) != '\0' || *(*buffer + ch) != '\r') && ch < *read_size) {
        *(*buffer + ch) = input_getc();
        ch++;
      }
      f->eax = ch;
    }  
    else if (*file_desc == 1 ||file_desc == NULL || cur->fdt[*file_desc] == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    else {
      f->eax = file_read(cur->fdt[*file_desc], *buffer, *read_size);
    }
    break;
  
  /*----------------------------------------------------------*/
  /*                          Write                           */
  /*----------------------------------------------------------*/
  case SYS_WRITE: // args: 3

    file_desc = pagedir_get_page(cur->pagedir, esp+4);        // arg0
    buffer = pagedir_get_page(cur->pagedir, esp+8);   // arg1
    int32_t * size = pagedir_get_page(cur->pagedir, esp+12);  // arg2

    if (*file_desc >= MAX_FD || buffer == NULL || *buffer == NULL || pagedir_get_page(cur->pagedir, *buffer) == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    else if(*file_desc == 1) {
      printf("%s", (char *)(*buffer));
      f->eax = *size;
    }
    else if (*file_desc == 0 ||file_desc == NULL || cur->fdt[*file_desc] == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    else {
      f->eax = file_write(cur->fdt[*file_desc], *buffer, *size);
    }
    
    break;
  
  /*----------------------------------------------------------*/
  /*                          Seek                            */
  /*----------------------------------------------------------*/
  case SYS_SEEK: // args: 2
    // printf("SYS_SEEK\n");
    file_desc = pagedir_get_page(cur->pagedir, esp+4);                // arg0
    int32_t * pos = pagedir_get_page(cur->pagedir, esp+8);            // arg1
    if (*file_desc == NULL || *file_desc >= MAX_FD || cur->fdt[*file_desc] == NULL || pos == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    file_seek(cur->fdt[*file_desc], *pos);
    break;
  
  /*----------------------------------------------------------*/
  /*                          Tell                            */
  /*----------------------------------------------------------*/
  case SYS_TELL: // args: 1
    // printf("SYS_TELL\n");
    if (*file_desc == NULL || *file_desc >= MAX_FD || cur->fdt[*file_desc] == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    f->eax = file_tell(cur->fdt[*file_desc]);
    break;
  
  /*----------------------------------------------------------*/
  /*                          Close                           */
  /*----------------------------------------------------------*/
  case SYS_CLOSE: // args: 1
    // printf("SYS_CLOSE\n");
    file_desc = pagedir_get_page(cur->pagedir, esp+4);                // arg0
    if (file_desc == NULL || *file_desc >= MAX_FD || cur->fdt[*file_desc] == NULL) {
      // cur->exit_status = -1;
      // thread_exit();
    }
    else {
      file_allow_write(cur->fdt[*file_desc]);
      file_close(cur->fdt[*file_desc]);
      cur->fdt[*file_desc] = NULL;
    }
    break;
  
  /*----------------------------------------------------------*/
  /*                  Default / Bad SYSCALL                   */
  /*----------------------------------------------------------*/
  default:
    // printf("Unknown condition: %d", *((char *)f->esp));
    cur->exit_status = -1; thread_exit();
    break;
  }


}

/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE. 
   Returns the byte value if successful, -1 if a segfault
   occurred. */

static int
get_user (const uint8_t *uaddr)
   {
       int result;
       asm ("movl $1f, %0; movzbl %1, %0; 1:"
              : "=&a" (result) : "m" (*uaddr));
       return result; 
   }

/* Writes BYTE to user address UDST.
    UDST must be below PHYS_BASE.
    Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
   int error_code;
   asm ("movl $1f, %0; movb %b2, %1; 1:"
           : "=&a" (error_code), "=m" (*udst) : "q" (byte));
   return error_code != -1;
             }
