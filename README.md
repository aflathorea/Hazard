# Hazard

How to win at loto 6/49, the easy way:<br>
1. Download the application and rawdata.txt from releases.<br>
2. Fill the rawdata.txt with sets corresponding to your lotery. You need the lasts 10 sets.
3. Run the application.<br>
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
This option does forecast. The run takes 5 minutes.<br>
Produces the output: H55_02.YYYY-MM-DD.txt. This is a summary file which lists all the horizons.<br>
A horizon means a group of sets having same statistic, 451 by example.<br>
There are others files which contain the sets for each horizon.<br>
If you do not know what to choose, choose the file H55_NEXT.YYYY-MM-DD_4510.txt and the firsts sets from the file.<br>
>hazard h<br>
This option does history analysis for last set. The run takes 5 minutes.<br>
Produces the output: H55_01.YYYY-MM-DD.txt.<br>
The rawdata.txt should contain 11 sets. It compares the first set, from the top of the file, against the others 10.<br>
Use this option to check the accuracy of your predictions.<br>
<br>
The application was written with Visual Studio Express 2012.<br>
<br>
How the algorithm is working?<br>
Generates all the sets (14 millions) and intersect each set with the 10 sets from rawdata.txt file.<br>
The intersection histogram represents an horizon.<br>
Based on the intersection histogram associate the sets with horizons.<br>
Inside the horizons compute the neighbors of each set.<br>
<br>
How to fill rawdata.txt<br>
The row zero is the header and it is ignored.<br>
The first row is the most recent set in timeline. The next row is previous in timeline, and so one.<br>
The mosts recents played are in the top and the sets played long time ago are in the bottom.<br>
There are needed the lasts 10 sets played to run the application.<br>
The structure of one row is splits in fields separated by comma.<br>
The first field is the date, it is readed as a string and it is not processed.<br>
The next 6 fields represents the numbers played for one set.<br>
You can comment a row into the file starting the row with: --. <br>
It is ignored a line like this:<br>
--2018-09-13,42,20,3,34,31,13<br>
<br>
The output of the application are:<br>
1. The summary file H55_02.YYYY-MM-DD.txt.<br>
2. Multiple files H55_NEXT.YYYY-MM-DD_NNNN.txt called horizon file and contains the sets associated with an horizon.<br>
<br>
The summary file H55_02.YYYY-MM-DD.txt contains the list of all the possible horizons.<br> 
It looks something like:<br>

[4|5|1|0|0|0]. 745437<br>
	(51,95859)(50,92895)(52,92176)(49,85872)(53,78607)(48,68809)(54,65477)(47,60979)(55,52425)(46,52338)<br>
[5|4|1|0|0|0]. 740001<br>
	(53,87620)(55,83351)(56,82600)(52,78869)(54,77948)(51,75280)(57,70398)(58,68536)(50,66290)(49,49109)<br>
[4|4|2|0|0|0]. 631219<br>
	(45,85095)(46,83282)(44,77671)(47,71317)(43,67162)(48,65903)(49,52011)(42,50431)(50,40253)(41,38094)<br>
...<br>
<br>
Basically each horizon has 2 rows: one row for horizon one row for neighbours.<br>
The first row has 2 fields: [4|5|1|0|0|0]. 745437. The first field is the horizon name and <br>
the second number is number of sets in horizon.<br>
The second row is for heighbours and has some pairs like: (51,95859)(50,92895)...<br>
A pair: the first number is the numbers of neighbours, the second numbers is the count of sets having same number of neighbours.<br>
Should be read like: <br>
There are 95859 having 51 neighbours.<br>
There are 92895 having 50 neighbours.<br>
At the firsts sight, the horizons like: [4|5|1|0|0|0], [5|4|1|0|0|0], [4|4|2|0|0|0] have a bigger number of sets and are favorites to winning.<br>
Basically the played sets can be in any horizon but there are favorite the horizons with bigger number of sets.<br>
This is the reason why most of the time the winning horizons are from the top.<br>
It is the user decision what horizon pick-up to play.<br>
<br>
The files like: H55_NEXT.YYYY-MM-DD_NNNN.txt contains the sets from horizon.<br>
The sets for horizon [4|5|1|0|0|0] are in file H55_NEXT.YYYY-MM-DD_4510.txt.<br>
The sets for horizon [5|4|1|0|0|0] are in file H55_NEXT.YYYY-MM-DD_5410.txt.<br>
The structure of a row in the file H55_NEXT.YYYY-MM-DD_NNNN.txt is:<br>
1-55. 12,13,23,28,32,41<br>
The first number 1, is the current index, just a counter.<br>
The second numbr 55 is the the number of neighbours.<br>
The lasts number is the set: 12,13,23,28,32,41.<br>
<br>
It is the user decision what horizons pick-up and what sets pick-up to play.<br>
There are favorites the horizon with a big numbers of sets and sets with a big number of neighbours.<br>
