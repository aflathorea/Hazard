# Hazard
How to win at loto 6/49, the easy way:<br>
1. Download the application and rawdata.txt from releases.<br>
2. Fill the rawdata.txt with sets corresponding to your lotery. You need the lasts 10 sets.
3. Run the application<br>
4. Open the file H55_NEXT.YYYY-MM-DD_4510.txt and select some sets from the top of the file.
<br>

Win at loto 6/49.<br>
The algorithm has 2 ideas:<br>
1. Rule 451, which says, the sets cannot combine infinitely and the combinations follow a normal distribution.
The combination 451 has the biggest chances to win. There is a picture in Docs/hazard_451.png which explains what means 451.<br>
2. Neighbours are sets having in common 5 elements. A winning set of 6 has around 250 sets of 5 elements.
In practice the sets having a big number of neighbours are favorites for winning.<br>
Please read hazard.pdf to understand the system.<br>
<br>
The application is written using C++11, so can be compiled platform independent, using cmake/make.<br>
To run the application, in the same directory, should be the file: rawdata.txt.<br>
The file should have, at least, 10 sets.<br>
<br>
Running options:<br>
>hazard<br>
This option does forecast. The run takes 1 minute.<br>
Produces the output: H55_02.YYYY-MM-DD.txt. This is a summary file which lists all the horizons.<br>
A horizon means a group of sets having same statistic, 451 by example.<br>
There are others files which contain the sets for each horizon.<br>
If you do not know what to choose, choose the file H55_NEXT.YYYY-MM-DD_4510.txt and the firsts sets from the file.<br>
>hazard h<br>
This option does history analysis for last set. The run takes 1 minute.<br>
Produces the output: H55_01.YYYY-MM-DD.txt.<br>
The rawdata.txt should contain 11 sets. It compares the 11th set against the others 10.<br>
Use this option to check the accuracy of your predictions.<br>
<br>
The application was developed with QTCreator, if you have installed QTCreator open the file Source/CMakeLists.txt.<br>
To compile the project:<br>
1. Open a terminal/command prompt.<br>
2. Navigate to Build directory. Now the current directory is Build.<br>
3. cmake ./../Source<br>
4. make<br>


