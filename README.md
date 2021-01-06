# Hazard

<dl>
<dt>What is Hazard?</dt>
<dd>The classical point of view, about the chances to win at loto 6/49, is that all the sets have the same chance to win, so the sets are distributed by the uniform distribution.<br>
I found a rule which says that some sets are favorite to the winning in front of the rest of sets, which transform the uniform distribution into a normal distribution.<br>
The rule is called Hazard and improve the chances to win at loto 6/49 by 10 times.</dd>
<dt>How the things are working?</dt>
<dd>What should we do to win at loto 6/49? We need to choose a winning set from a list of 14 millions sets. So, the basic model is a list of 14 millions sets, each set having the same chance to win like any other.<br>
The improvment I brought it, is that I grouped the sets into collections called horizons.<br> 
Instead of having a list of 14 millions sets, now we have a lists of 300 horizons and inside horizons we have sets. The advantage is that the horizons become sorted and some horizons are favorites to the winning much more than others horizons.<br> 
What is happening with the sets inside of an horizon? At this point we are going to tell that they have the same chance to win like any other set inside the horizon.</dd>
<dt>Rule 451</dt>
<dd>The horizons are shaped, using the histogram. To create the histogram:<br>
<ul>
<li>get the current set and intersect with the lasts 10 sets.</li>
<li>for each intersection count how many elements have in common.</li>
</ul>
Basically it is easy to see that 2 sets may have in common minim 0 elements and maxim 6 elements. Following the intersection, between the current set and the lasts 10, we get list like this one: [1,0,1,0,1,1,2,0,1,0]. In this list let's count how many 0s, 1s and 2s we have:<br>
<ul>	
<li>4 times 0</li>
<li>5 times 1</li>
<li>1 time 2</li>
</ul>
And we get the horizon 451: 0 by 4 times, 1 by 5 times, 2 by 1 time. First time when I applied the algorithm this was the first horizon that I discovered, which basically is the one having the biggest chances to win.<br>
The complete picture is to generate all the sets and put each set into the corresponding horizon. Doing this exercise can see check fast enough that horizon 451 is the winning one much more than others. In this moment the horizons are distributed using a normal distribution and certainly we are going to choose the horizons having a better chance to win than the others.<br>
How many horizons there are? There are around 300 horizons like: 451, 541, 820, 730, the secret is that the sum of the horizon numbers should be 10.<br>
How many sets are inside an horizon? The horizons contain a different number of sets, the biggest one like 451 or 541 contain 500,000 sets and the smaller like 190 have 700 sets.<br>
Here a sample, how are looking the horizons with their associated sets:<br>
<ul>
<li>[5|4|1|0|0|0]->523254</li>
<li>[4|5|1|0|0|0]->461353</li>
<li>[6|3|1|0|0|0]->446527</li>
<li>[4|4|2|0|0|0]->412252</li>
<li>[5|5|0|0|0|0]->358417</li>
<li>[5|3|2|0|0|0]->343375</li>
<li>[6|4|0|0|0|0]->310417</li>
<li>[3|6|1|0|0|0]->292944</li>
<li>[3|5|2|0|0|0]->250778</li>
<li>[4|6|0|0|0|0]->245152</li>
<li>[6|2|2|0|0|0]->215964</li>
<li>[7|2|1|0|0|0]->190956</li>
<li>[7|3|0|0|0|0]->156476</li>
<li>[4|3|3|0|0|0]->141547</li>
<li>[3|7|0|0|0|0]->140924</li>
<li>[3|4|3|0|0|0]->133262</li>
<li>[2|6|2|0|0|0]->98920</li>
<li>[2|7|1|0|0|0]->88037</li> 
<li>[5|2|3|0|0|0]->81149</li>
<li>[7|1|2|0|0|0]->66918</li> 
<li>[4|4|1|1|0|0]->66102</li> 
<li>[5|3|1|1|0|0]->62179</li> 
<li>[2|5|3|0|0|0]->62083</li> 
<li>[4|3|2|1|0|0]->50935</li> 
<li>[3|5|1|1|0|0]->45197</li> 
<li>[2|8|0|0|0|0]->43985</li> 
<li>[8|2|0|0|0|0]->35596</li> 
<li>[3|4|2|1|0|0]->34460</li> 
<li>[6|2|1|1|0|0]->28330</li> 
<li>[5|2|2|1|0|0]->19482</li> 
<li>[1|7|2|0|0|0]->16971</li> 
<li>[2|5|2|1|0|0]->14976</li> 
<li>[1|8|1|0|0|0]->11371</li> 
<li>[1|6|3|0|0|0]->10565</li> 
<li>[2|6|1|1|0|0]->9103</li> 
<li>[3|3|3|1|0|0]->8854</li> 
<li>[4|2|3|1|0|0]->8477</li> 
<li>[2|4|3|1|0|0]->7105</li> 
<li>[7|1|1|1|0|0]->4643</li> 
<li>[6|1|2|1|0|0]->1774</li> 
<li>[1|6|2|1|0|0]->1081</li> 
<li>[1|5|3|1|0|0]->945</li> 
<li>[1|9|0|0|0|0]->751</li>
</ul>
</dd> 
<dt>Neighbours</dt>
<dd>We are good, we have the horizons 451 and we know that it will get extracted enough often, so we can bet on it. But inside this horizon there are 500,000 sets which basically still means a lot. What should we do, to have a priority of sets inside the horizon?<br>
If we check the winning sets at loto 6/49 we can see that, there is one set with 6 numbers and 255 sets with 5 numbers. So this is the key to sort the sets into a priority list.<br>
We define a neigbhour of a set like another set having in common 5 numbers.<br>
Into the list of 500,000 sets we need to compute how many neighbours have each set nd the sets with a bigger number of neighbours are favorites to the winning.
So we are getting the first set from the list of 500,000 intersect with the rest of 499,999 sets and count only the sets which are neighbours, having in common 5 numbers. In the final sort the list descending, the sets with a bigger number of neighbours will be in top and the sets with a small number of neighbours at the end.</dd>
<dt>How to use it.</dt>
<dd>
<ol>
<li>Download the application and rawdata.txt from releases.</li>
<li>Fill the rawdata.txt with sets corresponding to your lotery. You need the lasts 10 sets.</li>
<li>Run the application.</li>
<li>Open the file H55_NEXT.YYYY-MM-DD_4510.txt and select some sets from the top of the file.</li>
</ol>
Running options:
<ul>
<li>The first option is to forecast the future having input the lasts 10 sets. This is the default option and you do not need any parameter to use this option. Have a file named rawdata.txt containing the lasts 10 sets, the most recent set is in top of the file.</li>
<li>The second option is to check the results having input the lasts 11 sets. It takes the firsts 10 sets and do exactly the same think like the above point. After that check where the 11th set matches the horizons. This option needs to pass the parameter -h to the application.</li>
</ul>
How to fill rawdata.txt:
<ul>
<li>The row zero is the header and it is ignored.</li>
<li>The first row is the most recent set in timeline. The next row is previous in timeline, and so one.</li>
<li>The mosts recents played are in the top and the sets played long time ago are in the bottom.</li>
<li>There are needed the lasts 10 sets played to run the application.</li>
<li>The structure of one row is splited in fields separated by comma.</li>
<li>The first field is the date, it is readed as a string and it is not processed.</li>
<li>The next 6 fields represents the numbers played for one set.</li>
<li>You can comment a row into the file starting the row with: --. </li>
<li>It is ignored a line like this:--2018-09-13,42,20,3,34,31,13</li>
</ul>
The outputs of the application are:
<ol>
<li>The summary file H55_02.YYYY-MM-DD.txt.</li>
<li>Multiple files H55_NEXT.YYYY-MM-DD_NNNN.txt called horizon file and contains the sets associated with an horizon.</li>
</ol>
The summary file H55_02.YYYY-MM-DD.txt contains the list of all the possible horizons. 
<ul>
<li>[4|5|1|0|0|0]. 745437<br>
	(51,95859)(50,92895)(52,92176)(49,85872)(53,78607)(48,68809)(54,65477)(47,60979)(55,52425)(46,52338)</li>
<li>[5|4|1|0|0|0]. 740001<br>
	(53,87620)(55,83351)(56,82600)(52,78869)(54,77948)(51,75280)(57,70398)(58,68536)(50,66290)(49,49109)</li>
<li>[4|4|2|0|0|0]. 631219<br>
	(45,85095)(46,83282)(44,77671)(47,71317)(43,67162)(48,65903)(49,52011)(42,50431)(50,40253)(41,38094)</li>
</ul>
Basically each horizon has 2 rows: 
<ul>
<li>one row for horizon</li>
<li>one row for neighbours.</li>
</ul>
The first row has 2 fields: [4|5|1|0|0|0]. 745437. 
<ul>
<li>The first field is the horizon name and </li>
<li>The second number is number of sets in horizon.</li>
</ul>	
The second row is for heighbours and has some pairs like: (51,95859)(50,92895)...<br>
A pair: the first number is the numbers of neighbours, the second numbers is the count of sets having same number of neighbours.
<ul>
<li>There are 95859 having 51 neighbours.</li>
<li>There are 92895 having 50 neighbours.</li>
</ul>
At the firsts sight, the horizons having a bigger number of sets are favorites to winning:
<ul>
<li>[4|5|1|0|0|0]</li>
<li>[5|4|1|0|0|0]</li>
<li>[4|4|2|0|0|0]</li>
</ul>
Basically the played sets can be in any horizon but there are favorite the horizons with bigger number of sets.<br>
This is the reason why most of the time the winning horizons are from the top.<br>
It is the user decision what horizon pick-up to play.<br>
<br>
The files like: H55_NEXT.YYYY-MM-DD_NNNN.txt contains the sets from horizon.
<ul>
<li>The sets for horizon [4|5|1|0|0|0] are in file H55_NEXT.YYYY-MM-DD_4510.txt.</li>
<li>The sets for horizon [5|4|1|0|0|0] are in file H55_NEXT.YYYY-MM-DD_5410.txt.</li>
</ul>
The structure of a row in the file H55_NEXT.YYYY-MM-DD_NNNN.txt is:
<ul>
<li>1-55. 12,13,23,28,32,41</li>
<li>The first number 1, is the current index, just a counter.</li>
<li>The second numbr 55 is the the number of neighbours.</li>
<li>The lasts number is the set: 12,13,23,28,32,41.</li>
</ul>
It is the user decision what horizons pick-up and what sets pick-up to play.<br>
There are favorites the horizon with a big numbers of sets and sets with a big number of neighbours.<br>
</dd>
</dl>
