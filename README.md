TinyMob OS 


This project is a firmware-like operating system in the very early stages of development, built using C and GTK for efficiency. Running on top of the NetBSD kernel, it’s designed for touch based feature phones and low-resource devices. The primary motive of this project is to create a non-distracting, minimalistic OS that helps users stay focused. With a no-bloat philosophy, it only includes essential components to ensure a fast and lightweight experience, providing a clean and straightforward environment for productive use.

## Install gtk and gcc



```bash
  sudo apt install build-essential
  sudo apt install libgtk-3-dev

```



    
## To compile 



```bash
  sudo gcc -o main main.c $(pkg-config --cflags --libs gtk-3.0) && ./main![calc](https://github.com/user-attachments/assets/586d2b6c-8ddf-423d-9dee-c1af6a8e8595)


```

![lockscreen](https://github.com/user-attachments/assets/826ba1b3-9524-49cc-924d-d38813971b33)
![home](https://github.com/user-attachments/assets/208b0943-14d7-4de7-a3ad-b183e6e49df7)
![calc](https://github.com/user-attachments/assets/624da745-c810-48b8-95f4-4aa38ac4ab01)
