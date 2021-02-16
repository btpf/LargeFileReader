# LargeFileReader (Work In Progress)

LargeFileReader is an a cross platform low resource file reader written with QT Creator in C++. Inspired by Total Commander's *Lister* Tool.

The application was created because most text editors and viewers struggle to open files exceeding 1GB. To solve this issue, LargeFileReader will only read text into memory that can be fit in the display.  This results in a smooth and response viewing experience.

LargeFileReader is also capable of quickly searching patterns and text through the use of Intel's Hyperscan library. 



![2021-02-16_06-05](https://user-images.githubusercontent.com/61168382/108064198-5318be80-702a-11eb-931d-39bd988b4b28.png)



Currently, this application is only capable of reading ASCII text, but could expand to support other encodings in the future.
