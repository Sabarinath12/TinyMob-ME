TinyMob OS 


This project is a firmware-like operating system in the very early stages of development, built using C and GTK for efficiency. Running on top of the NetBSD kernel, it’s designed for touch based feature phones and low-resource devices. The primary motive of this project is to create a non-distracting, minimalistic OS that helps users stay focused. With a no-bloat philosophy, it only includes essential components to ensure a fast and lightweight experience, providing a clean and straightforward environment for productive use.

## Install gtk and gcc



```bash
  sudo apt install build-essential
  sudo apt install libgtk-3-dev

```



    
## To compile 



```bash
  sudo gcc -o main main.c $(pkg-config --cflags --libs gtk-3.0) && ./main

```





![lock](https://github.com/user-attachments/assets/a79c15d1-a39c-4bf2-976c-bae9a29717c6)

![calc](https://github.com/user-attachments/assets/624da745-c810-48b8-95f4-4aa38ac4ab01)
![home](https://github.com/user-attachments/assets/79e2016e-c1c3-4eb5-a602-b21d37870968)
