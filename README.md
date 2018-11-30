# Hazard
Win a loto 6/49.<br>
The algorithm has 2 ideas:<br>
1. Rule 451, which says that the sets cannot combine infinitely and the combinations follows a normal distribution.<br>
2. Neighours, which says that the sets having a big number of neighbours in common 5 elements are favorites for winning.<br>
Please read hazard.pdf to understand the system.<br>
<br>
The application is using Qt. You need to install the Qt:<br>
https://www.qt.io/download-qt-installer?hsCtaTracking=9f6a2170-a938-42df-a8e2-a9f0b1d6cdce%7C6cb0de4f-9bb5-4778-ab02-bfb62735f3e5<br>
In the app dir should be the file: rawdata.txt. The file should have, at least, the lasts 10 sets.<br>
Run Hazard app<br>
H55_HIST_04.YYYY-MM-DD.txt contains the analysis for the last set, in the case there are 11 sets in rawdata.txt file.<br>
H55_NEXT_SUMMARY.YYYY-MM-DD.txt contains the summary forecast: the sets enumeration with the corresponding neighbours.<br>
H55_NEXT.YYYY-MM-DD_4510.txt<br> contains the sets for this horizon.

