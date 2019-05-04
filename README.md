# Hazard
Win a loto 6/49.<br>
The algorithm has 2 ideas:<br>
1. Rule 451, which says, the sets cannot combine infinitely and the combinations follow a normal distribution.<br>
2. Neighbours are defined other sets having in common 5 elements with current one. The sets having a big number of neighbours are favorites for winning.<br>
Please read hazard.pdf to understand the system.<br>
<br>
The application is written using C++11 and can be compiled with make.
In the app dir should be the file: rawdata.txt. The file should have, at least, the lasts 10 sets.<br>
Run Hazard app<br>
H55_HIST_04.YYYY-MM-DD.txt contains the analysis for the last set, in the case there are 11 sets in rawdata.txt file.<br>
H55_NEXT_SUMMARY.YYYY-MM-DD.txt contains the summary forecast: the sets enumeration with the corresponding neighbours.<br>
H55_NEXT.YYYY-MM-DD_4510.txt<br> contains the sets for this horizon.

