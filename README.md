## Image Converter (ICV)
This is a program which converts jpg/png image to pixel data for mangoboard.

### Prerequisites
ICV needs OpenCV(>=3) to analyze image pixels. You can type one of below to install OpenCV.
```
sudo apt-get install libopencv-dev
sudo yum install opencv
sudo pacman -S opencv
```

### Build
First, clone this repository.
```
git clone -b image-conversion http://github.com/Yuoa/Pacman
```
You can now build ICV with command below.
```
make
```
If you want use ICV on everywhere, you can install it.
```
make install
```

### Test
After you build ICV, you can test it with
```
make test-d
```
or
```
make test-i
```
You can see the result of these commands on "test.out" file.
If it is exactly same with "test-ref.out", you built ICV successfully.

### Notes
- OpenCV does not support GIF. So, ICV also doesn't support GIF.
- If you enter directory path as argument, ICV will not explore your directory recursively.

### License
ICV follows GPLv3.
