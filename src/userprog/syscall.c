#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "pagedir.h"
#include "filesys/filesys.h"

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
  char ** filename;

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

  switch (*((char *)f->esp))
  {
  /*----------------------------------------------------------*/
  /*                          HALT                            */
  /*----------------------------------------------------------*/
  case SYS_HALT: // args: 0
    // TODO: Need to implement halting properly
    // printf("SYS_HALT\n");
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
    // const char **file = (esp + 4) >= 0xbffffffc ? NULL : esp + 4;
    const char **file = *((const char *) pagedir_get_page(cur->pagedir, esp+4));
    if (file == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    break;
  
  /*----------------------------------------------------------*/
  /*                          Wait                            */
  /*----------------------------------------------------------*/
  case SYS_WAIT: // args: 1
    printf("SYS_WAIT\n");
    break;
  
  /*----------------------------------------------------------*/
  /*                         Create                           */
  /*----------------------------------------------------------*/
  case SYS_CREATE: ;// args: 2
    // printf("SYS_CREATE\n");
    
    // char ** new_file = pagedir_get_page(cur->pagedir, esp+4);      // arg0
    filename = pagedir_get_page(cur->pagedir, esp+4);                 // arg0
    unsigned * init_size = pagedir_get_page(cur->pagedir, esp+8);     // arg1
    if (*filename == NULL || init_size == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }
    f->eax = (uint32_t) filesys_create(*filename, *init_size);

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
    if (*filename == NULL) {
      cur->exit_status = -1;
      thread_exit();
    }

    for(int i = 2; i<10; i++) {
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
    printf("SYS_FILESIZE\n");
    break;
  
  /*----------------------------------------------------------*/
  /*                          Read                            */
  /*----------------------------------------------------------*/
  case SYS_READ: // args: 3
    printf("SYS_READ\n");
    break;
  
  /*----------------------------------------------------------*/
  /*                          Write                           */
  /*----------------------------------------------------------*/
  case SYS_WRITE:; // args: 3

    int32_t * fd = pagedir_get_page(cur->pagedir, esp+4);     // arg0
    char ** buffer = pagedir_get_page(cur->pagedir, esp+8);   // arg1
    int32_t * size = pagedir_get_page(cur->pagedir, esp+12);  // arg2
    
    // int32_t * fd = esp + 4;     // arg1
    // char ** buffer = esp + 8;   // arg1
    // int32_t * size = esp + 12;  // arg2

    printf("%s", (char *)(*buffer));
    break;
  
  /*----------------------------------------------------------*/
  /*                          Seek                            */
  /*----------------------------------------------------------*/
  case SYS_SEEK: // args: 2
    printf("SYS_SEEK\n");
    break;
  
  /*----------------------------------------------------------*/
  /*                          Tell                            */
  /*----------------------------------------------------------*/
  case SYS_TELL: // args: 1
    printf("SYS_TELL\n");
    break;
  
  /*----------------------------------------------------------*/
  /*                          Close                           */
  /*----------------------------------------------------------*/
  case SYS_CLOSE: // args: 1
    // printf("SYS_CLOSE\n");
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
