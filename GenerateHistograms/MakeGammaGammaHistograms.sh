#!/bin/bash

ANALDIR=/tig/belodon_data4/S9038/S9038-Nov2019/AnalysisTrees
HISTDIR=/tig/belodon_data4/S9038/Projects/GammaGammaAnalysis/GenerateHistograms/myAnalysis/Histograms
DLEN=${#ANALDIR}
SORTCODE=/tig/belodon_data4/S9038/Projects/GammaGammaAnalysis/GenerateHistograms/myBuild/GammaGammaAnalysis

# calibration files
CALFILE=/tig/belodon_data4/S9038/S9038-Nov2019/PreSrcCal.cal
#CFILE=/tig/belodon_data4/S9038/PostSrcCal.cal

m=15600 #Only Sort after this run First good run: 52042
n=99999 #Only Sort until this run
BAD_RUN_LIST=(15719 15720)

#############################################
# Helper functions
#############################################

# checks if item is in list
bad_run_check() {
   [[ $1 =~ (^|[[:space:]])$2($|[[:space:]]) ]] && return 0 || return 1
}

#############################################
# Main
#############################################

if [ $# -eq 0 ]
then
    echo "0 argument(s) not implemented yet"
   #for f in $ANALDIR/*.root
#
   #do
   #   #echo "$f"
   #   g=${f:DLEN+9}
   #   h=${g:0:${#g}-6}
   #   i=${g:0:${#g}-9} # run number
#
   #   RUNFILES=$ANALDIR/analysis${i}_"*".root
   #   HFILE=$HISTDIR/TwoPhotonGriffinAnglePlots${i}_"*".root
#
   #   if [ $i -gt $m ] && [ $n -gt $i ];
   #   then
   #      if [ ! -f $HFILE ];
   #      then
   #         echo "$SORTCODE $RUNFILES $SELECTOR --max-workers=4"
   #         #$SORTCODE $RUNFILES $SELECTOR --max-workers=4
   #         mv ./TwoPhotonGriffinAnglePlots* $HISTDIR/
   #      fi
   #      if [ -f $HFILE ]
   #      then
   #         # if the sort code gets updated or the analysis tree is updated
   #         if [ $HFILE -ot $SORTCODE ] || [ $HFILE -ot $f ];
   #         then
   #            echo "$SORTCODE $RUNFILES $SELECTOR --max-workers=4"
   #            #$SORTCODE $RUNFILES $SELECTOR --max-workers=4
   #            mv TwoPhotonGriffinAnglePlots* $HISTDIR/
   #         fi
   #      fi
   #   fi
   #done
fi

# for a single run
if [ $# -eq 1 ]
then
    file=$ANALDIR/analysis"$1"_*.root;
    g=${file:DLEN+9}
    h=${g:0:${#g}-6}
    i=${g:0:${#g}-7} # run number

      RUNFILES=$ANALDIR/analysis${i}_"*".root
      HFILE=$HISTDIR/gg_${i}.root

      if [ $i -gt $m ] && [ $n -gt $i ];
      then
         if [ ! -f $HFILE ];
         then
           # echo "$SORTCODE $RUNFILES $CALFILE"
           # echo "mv gg_* $HISTDIR/"
           $SORTCODE $RUNFILES $CALFILE
           mv gg_* $HISTDIR/gg_${i}_total.root

         fi
         if [ -f $HFILE ]
         then
            # if the sort code gets updated or the analysis tree is updated
            if [ $HFILE -ot $SORTCODE ] || [ $HFILE -ot $f ];
            then
               echo "Sort code or analysis tree have changed, rerunning sort"
                echo "$SORTCODE $RUNFILES $CALFILE"
                echo "mv gg_* $HISTDIR/"
               #$SORTCODE $RUNFILES $SELECTOR --max-workers=4
               #mv TwoPhotonGriffinAnglePlots* $HISTDIR/
            fi
         fi
     elif [[ $i < $m ]]; then
         echo "Run $1 is less than the low run cutoff: $m"
         echo "Exiting ..."
     else
         echo "$1 is non accepted run format"
         echo "Exiting ..."
      fi
fi

# for a range of runs
if [ $# -eq 2 ] # Histogram runs between the two input values
then
    echo "2 argument(s) not implemented yet"
   #for ((i=$1; i<=$2; ++i));
   #do
   #   # checks for bad run
   #   #bad_run_check $BAD_RUN_LIST $i && continue
#
   #   RUNFILES=$ANALDIR/analysis${i}_"*".root
   #   HFILE=$HISTDIR/TwoPhotonGriffinAnglePlots${i}_"*".root
   #   echo $HFILE
#
   #   if [ $i -gt $m ] && [ $n -gt $i ];
   #   then
   #      if [ ! -f $HFILE ];
   #      then
   #         echo "$SORTCODE $RUNFILES $SELECTOR --max-workers=4"
   #         $SORTCODE $RUNFILES $SELECTOR --max-workers=4
   #         mv TwoPhotonGriffinAnglePlots* $HISTDIR/
   #      fi
#  #       if [ -f $HFILE ]
#  #       then
#  #          # if the sort code gets updated or the analysis tree is updated
#  #          if [ $HFILE -ot $SORTCODE ] || [ $HFILE -ot $f ];
#  #          then
#  #             echo "$SORTCODE $RUNFILES $SELECTOR --max-workers=4"
#  #             #$SORTCODE $RUNFILES $SELECTOR --max-workers=4
#  #             mv TwoPhotonGriffinAnglePlots* $HISTDIR/
#  #          fi
#  #       fi
   #   fi
   #done

fi

if [[ $# > 2 ]]
then
   echo "Pass 0, 1, or 2 arguments"
fi

echo "Done"
