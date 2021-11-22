# Branch_Pred

Branch Predictor Problem statement : construct a branch predictor simulator and use it to evaluate different
configurations of branch predictors.


Modeled a gshare branch predictor with parameters {m, n}, where:
 m is the number of low-order PC bits used to form the prediction table index. Note: discard
the lowest two bits of the PC, since these are always zero, i.e., use bits m+1 through 2 of the
PC.
 n is the number of bits in the global branch history register. Note: n ≤ m. Note: n may equal
zero, in which case we have the simple bimodal branch predictor.
3.1.1. n=0: bimodal branch predictor
When n=0, the gshare predictor reduces to a simple bimodal predictor. In this case, the index is
based on only the branch’s PC
Entry in the prediction table:
An entry in the prediction table contains a single 2-bit counter. All entries in the prediction table
should be initialized to 2 (“weakly taken”) when the simulation begins.
Regarding branch interference:
Different branches may index the same entry in the prediction table. This is called “interference”.
Interference is not explicitly detected or avoided: it just happens. (There is no tag array, no tag
checking, and no “miss” signal for the prediction table!)
Steps:
When you get a branch, there are three steps:
(1) Determine the branch’s index into the prediction table.
(2) Make a prediction. Use index to get the branch’s counter from the prediction table. If the
counter value is greater than or equal to 2, then the branch is predicted taken, else it is predicted
not-taken.
(3) Update the branch predictor based on the branch’s actual outcome. The branch’s counter in
the prediction table is incremented if the branch was taken, decremented if the branch was nottaken. The counter saturates at the extremes (0 and 3), however.

3.1.2. n>0: gshare branch predictor
When n > 0, there is an n-bit global branch history register. In this case, the index is based on
both the branch’s PC and the global branch history register, as shown in Fig. 2 below. The global
branch history register is initialized to all zeroes (00...0) at the beginning of the simulation.
Steps:
When you get a branch from the trace file, there are four steps:
(1) Determine the branch’s index into the prediction table. Fig. 2 shows how to generate the
index: the current n-bit global branch history register is XORed with the uppermost n bits of the
m PC bits.
(2) Make a prediction. Use index to get the branch’s counter from the prediction table. If the
counter value is greater than or equal to 2, then the branch is predicted taken, else it is predicted
not-taken.
(3) Update the branch predictor based on the branch’s actual outcome. The branch’s counter in
the prediction table is incremented if the branch was taken, decremented if the branch was nottaken. The counter saturates at the extremes (0 and 3), however.
(4) Update the global branch history register. Shift the register right by 1 bit position, and place
the branch’s actual outcome into the most-significant bit position of the register.

 Hybrid branch predictor
Model a hybrid predictor that selects between the bimodal and the gshare predictors, using a
chooser table of 2k 2-bit counters. All counters in the chooser table are initialized to 1 at the
beginning of the simulation.
Steps:
When you get a branch from the trace file, there are six top-level steps:
(1) Obtain two predictions, one from the gshare predictor (follow steps 1 and 2 in Section 3.1.2)
and one from the bimodal predictor (follow steps 1 and 2 in Section 3.1.1).
(2) Determine the branch’s index into the chooser table. The index for the chooser table is bit
k+1 to bit 2 of the branch PC (i.e., as before, discard the lowest two bits of the PC).
(3) Make an overall prediction. Use index to get the branch’s chooser counter from the chooser
table. If the chooser counter value is greater than or equal to 2, then use the prediction that was
obtained from the gshare predictor, otherwise use the prediction that was obtained from the
bimodal predictor.
(4) Update the selected branch predictor based on the branch’s actual outcome. Only the branch
predictor that was selected in step 3, above, is updated 
(5) Note that the gshare’s global branch history register must always be updated, even if bimodal
was selected (
