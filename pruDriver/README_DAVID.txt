Step 1: Get the development tools for working with the PRU
  a. apt-get install device-tree-compiler  // This is the compiler for device trees
  b. Follow mythopoeic guide to installing the pru assembler "PASM" on the website below (Only follow the "Development Tools section)
  c. go to github.com/beagleboard/bb.org-overlays and follow their steps to pull their dts files (This does not include the hcsr04 overlay but does include some other useful ones. After getting the dts files you will also need to compile them into .dtbo files and move them to /lib/firmware

Step 2: Wire the sonar
  a. Follow the .png image to wire the sonar

Step 3: Make and run
  a. in cd ~/pruDriver. Type: make	// This compiles the c and assembly programs
  b. Type: make install		// This initializes the pru cape overlay
  c. Type: ./hcsr04	// The hcsr04 shoud run

Sources:
www.mythopoeic.org/bbb-pru-minimal
https://github.com/luigif/hcsr04
