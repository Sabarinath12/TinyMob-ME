TinyMob OS 


This project is a firmware-like operating system in the very early stages of development, built using C and GTK for efficiency. Running on top of the FreeBSD , it’s designed for touch based feature phones and low-resource devices. The primary motive of this project is to create a non-distracting, minimalistic OS that helps users stay focused. With a no-bloat philosophy, it only includes essential components to ensure a fast and lightweight experience, providing a clean and straightforward environment for productive use.

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


![home_v2](https://github.com/user-attachments/assets/0a39db26-5d89-4178-bd23-1333ff69ff47)

![calc](https://github.com/user-attachments/assets/66cad04f-a864-4fa2-9baa-5ca5d1cd0967)


