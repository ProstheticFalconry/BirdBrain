#include <linux/init.h>           // Macros used to mark up functions e.g. __init __exit
#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/device.h>         // Header to support the kernel Driver Model
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <linux/string.h>
#include <asm/uaccess.h>          // Required for the copy to user functioni
#include "lib/ringBuf.h"


#define MAX_BUF_SIZE 1024
#define CHAN_BUF_SIZE 10

#define MAJOR_NUMBER 0

#define FLY0_MN 38
#define THROT_MN 39
#define ROLL_MN 40
#define PITCH_MN 41
#define YAW_MN 42
#define MODE_MN 43 

#define ASCII_F 102
#define ASCII_T 116
#define ASCII_R 114
#define ASCII_P 112
#define ASCII_Y 121
#define ASCII_M 109

#define FDIR "fly/"

#define TEST "test"

#define DEVICE_NAME FDIR "fly0"
#define THROTTLE_CHAN FDIR "throttle"
#define ROLL_CHAN FDIR "roll"
#define PITCH_CHAN FDIR "pitch"
#define YAW_CHAN FDIR "yaw"
#define MODE_CHAN FDIR "mode"
#define CLASS_NAME  "flight_cntrl"        ///< The device class -- this is a character device driver
 
MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Mihai Esanu");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Provides a series of I/O buffers for communication with the RT stack");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users
 
static int    majorNumber;                  ///< Stores the device number -- determined automatically

static RingBuf *mainBuf;

static char chanMessage[CHAN_BUF_SIZE]={0};
static char chanInBuf[CHAN_BUF_SIZE]={0};
static char chanOutBuf[CHAN_BUF_SIZE]={0};

static struct class*  flightClass  = NULL; ///< The device-driver class struct pointer
static struct device* fly0Device = NULL; ///< The device-driver device struct pointer
static struct device* throtDevice = NULL;
static struct device* pitchDevice = NULL;
static struct device* rollDevice = NULL;
static struct device* yawDevice = NULL;
static struct device* modeDevice = NULL;
// The prototype functions for the character driver -- must come before the struct definition
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

int register_devices(int majorNumber);
void destroy_devices(int majorNumber);
void deregister_devices(int majorNumber);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init flightControl_init(void){
   printk(KERN_INFO ": Initializing the FlightControl LKM\n");
 
   // Try to dynamically allocate a major number for the device -- more difficult but worth it
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "FlightControl failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "FlightControl: registered correctly with major number %d\n", majorNumber);
 
   // Register the device class
   flightClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(flightClass)){                // Check for error and clean up if there is
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Failed to register device class\n");
      return PTR_ERR(flightClass);          // Correct way to return an error on a pointer
   }
   printk(KERN_INFO "FlightControl: device class registered correctly\n");

   if (register_devices(majorNumber)){               // Clean up if there is an error
      class_destroy(flightClass);           // Repeated code but the alternative is goto statements
      deregister_devices(majorNumber);
      printk(KERN_ALERT "Failed to create the device\n");
      return 1;
   }
   printk(KERN_INFO "FlightControl: device class created correctly\n"); // Made it! device was initialized
   mainBuf=bufInit(MAX_BUF_SIZE);
   if(!mainBuf){
   	printk(KERN_ALERT "Couldn't allocated ringbuf\n");
   }
   return 0;
}


/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit flightControl_exit(void){
   bufFree(mainBuf);
   destroy_devices(majorNumber);
   class_unregister(flightClass);                          // unregister the device class
   class_destroy(flightClass);                             // remove the device class
   deregister_devices(majorNumber);             // unregister the major number
   printk(KERN_INFO "FlightControl: Goodbye from the LKM!\n");
}
 
/** @brief The device open function that is called each time the device is opened
 *  This will only increment the numberOpens counter in this case.
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_open(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "FlightControl: Device has been opened");
   return 0;
}
 
/** @brief This function is called whenever device is being read from user space i.e. data is
 *  being sent from the device to the user. In this case is uses the copy_to_user() function to
 *  send the buffer string to the user and captures any errors.
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 *  @param buffer The pointer to the buffer to which this function writes the data
 *  @param len The length of the b
 *  @param offset The offset if required
 */
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   char firstLetter;
   unsigned int msgSize;
   unsigned int err;
   firstLetter = filep->f_path.dentry->d_iname[0];
   if (firstLetter == ASCII_F){
	printk(KERN_INFO "mainBuf has %u characters in use", (unsigned int)mainBuf->bufUse);
   	msgSize=(unsigned int)bufPull(mainBuf,chanOutBuf,CHAN_BUF_SIZE);
	printk(KERN_INFO "pulling from ringbuf\n%s\n%u characters pulled",chanOutBuf,msgSize);
	err=copy_to_user(buffer,chanOutBuf,msgSize);
	printk(KERN_INFO "produced %u errors while copying to user",err);
	return msgSize;	
   }
   return -EFAULT;
}
 
/** @brief This function is called whenever the device is being written to from user space i.e.
 *  data is sent to the device from the user. The data is copied to the message[] array in this
 *  LKM using the sprintf() function along with the length of the string.
 *  @param filep A pointer to a file object
 *  @param buffer The buffer to that contains the string to write to the device
 *  @param len The length of the array of data that is being passed in the const char buffer
 *  @param offset The offset if required
 */
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   char firstLetter;
   
   firstLetter=filep->f_path.dentry->d_iname[0]; 
      
   switch(firstLetter){
   	case ASCII_R:
		chanInBuf[0]=ROLL_MN;
		break;
	case ASCII_P:
		chanInBuf[0]=PITCH_MN;
		break;
	case ASCII_Y:
		chanInBuf[0]=YAW_MN;
		break;
	case ASCII_T:
		chanInBuf[0]=THROT_MN;
		break;
	case ASCII_M:
		//chanMessage[0]=MODE_MN;
		break;
	case ASCII_F:
		break;
	default:
		break;
   }
   if (len >= CHAN_BUF_SIZE-1)
        return -EFAULT;
   copy_from_user(chanMessage,buffer,len);
   strcpy((chanInBuf+1),chanMessage);
   bufPush(mainBuf,chanInBuf);
   printk(KERN_INFO "FlightControl: Woopsies Received %zu characters from the user\n", len);
   return len;
}
 
/** @brief The device release function that is called whenever the device is closed/released by
 *  the userspace program
 *  @param inodep A pointer to an inode object (defined in linux/fs.h)
 *  @param filep A pointer to a file object (defined in linux/fs.h)
 */
static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "FlightControl: Device successfully closed\n");
   return 0;
}
 
int register_devices(int majorNumber){
   fly0Device = device_create(flightClass, NULL, MKDEV(majorNumber, FLY0_MN), NULL, DEVICE_NAME);
   throtDevice = device_create(flightClass, NULL, MKDEV(majorNumber, THROT_MN), NULL, THROTTLE_CHAN);
   pitchDevice = device_create(flightClass, NULL, MKDEV(majorNumber, PITCH_MN), NULL, PITCH_CHAN);
   rollDevice = device_create(flightClass, NULL, MKDEV(majorNumber, ROLL_MN), NULL, ROLL_CHAN);
   yawDevice = device_create(flightClass, NULL, MKDEV(majorNumber,YAW_MN), NULL, YAW_CHAN);
   modeDevice = device_create(flightClass, NULL, MKDEV(majorNumber, MODE_MN), NULL, MODE_CHAN);
   return (IS_ERR(fly0Device) || IS_ERR(throtDevice) || IS_ERR(pitchDevice) ||IS_ERR(rollDevice) || IS_ERR(yawDevice) || IS_ERR(modeDevice));
}

void destroy_devices(int majorNumber){
	device_destroy(flightClass, MKDEV(majorNumber, FLY0_MN));
	device_destroy(flightClass, MKDEV(majorNumber, THROT_MN));
	device_destroy(flightClass, MKDEV(majorNumber, PITCH_MN));
	device_destroy(flightClass, MKDEV(majorNumber, ROLL_MN));
	device_destroy(flightClass, MKDEV(majorNumber, YAW_MN));
	device_destroy(flightClass, MKDEV(majorNumber, MODE_MN));
}

void deregister_devices(int majorNumber){
   unregister_chrdev(majorNumber, DEVICE_NAME);
   unregister_chrdev(majorNumber, THROTTLE_CHAN);
   unregister_chrdev(majorNumber, PITCH_CHAN);
   unregister_chrdev(majorNumber, ROLL_CHAN);
   unregister_chrdev(majorNumber, YAW_CHAN);
   unregister_chrdev(majorNumber, MODE_CHAN);
}

/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(flightControl_init);
module_exit(flightControl_exit);
