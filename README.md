# Tucil1_13523086_13523104
---

# Quadtree Image Compression

## 📌 Overview

This program performs image compression using the **Quadtree** data structure, with support for multiple error metrics:  
- **Variance**  
- **Mean Absolute Deviation (MAD)**  
- **Max Pixel Difference**  
- **Entropy**  
- **Structural Similarity Index (SSIM)** 

It also supports a **target compression ratio** and visualizes the compression process through an output **GIF animation**.

---

## 🛠 Requirements & Installation

Make sure you use the **latest C++ compiler (C++17 or newer)**.  
Tested on **Windows with MSYS2** environment.

### Install required dependencies:
```bash
pacman -S mingw-w64-ucrt-x86_64-imagemagick
```

> `ImageMagick` is used to generate GIFs from PNG or JPG image sequences.

---

## ⚙️ How to Compile

To compile the program:
```bash
make
```

To run the program after compiling:
```bash
make run
```

---

## ▶️ How to Use

1. Run the program using `make run`.
2. Enter the **absolute path** to the image you want to compress (e.g. `C:\Users\User\test\INPUT.jpg`).
3. Choose one of the error metrics (1–5).
4. Enter the **error threshold** and **minimum block size**.
5. (Optional) Enter the **target compression ratio** (e.g. `30` for 30% compression or `0` to disable it).
6. Provide the **absolute path** for the compressed image output (e.g. `C:\Users\User\test\OUTPUT.jpg`).
7. Provide the **absolute path** for the GIF output (must end with `.gif` and contain no spaces).

---

## 👤 Author

### Bob Kunanda  
- **NIM:** 13523086  
- **Email:** bobkunanda@gmail.com  

### M. Zahran Ramadhan Ardiana  
- **NIM:** 13523104  
- **Email:** muzarcode@gmail.com  

**Course:** IF2211 - Algorithm Strategy  
**Assignment:** Tugas Kecil 2 - Quadtree Image Compression
