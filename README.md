TinyMob OS 


This project is a operating system in the very early stages of development, built using C and GTK for efficiency. Running on top of the FreeBSD , it’s designed for touch based feature phones and low-resource devices. The primary motive of this project is to create a non-distracting, minimalistic OS that helps users stay focused. With a no-bloat philosophy, it only includes essential components to ensure a fast and lightweight experience, providing a clean and straightforward environment for productive use.

## Install gtk and gcc



```bash
  sudo apt install build-essential
  sudo apt install libgtk-3-dev

```



    
## To compile 



```bash
  sudo gcc -o main main.c $(pkg-config --cflags --libs gtk-3.0) && ./main

```






![lockscreen](https://github.com/user-attachments/assets/6fdbc13d-c1f5-4bed-b88e-3e18214b06d1)

![home](https://github.com/user-attachments/assets/507a5f0b-b82c-4f80-b05c-2526ffe93064)




