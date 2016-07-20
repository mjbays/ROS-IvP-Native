#!/bin/bash 
#-------------------------------------------------------
#  Part 1: Check for and handle command-line arguments
#-------------------------------------------------------
TIME_WARP=1
JUST_MAKE="no"
COOL_FAC=50
COOL_STEPS=1000
CONCURRENT="true"
ADAPTIVE="false"
SURVEY_X=70
SURVEY_Y=-100
HEIGHT1=150
HEIGHT2=150
WIDTH1=120
WIDTH2=120
LANE_WIDTH1=25
LANE_WIDTH2=25
DEGREES1=270
DEGREES2=0
for ARGI; do
    #help:
    if [ "${ARGI}" = "--help" -o "${ARGI}" = "-h" ] ; then 
        HELP="yes"
        UNDEFINED_ARG=""	
    #time warmp
    elif [ "${ARGI//[^0-9]/}" = "$ARGI" -a "$TIME_WARP" = 1 ]; then 
        TIME_WARP=$ARGI
    elif [ "${ARGI:0:6}" = "--warp" ] ; then
        WARP="${ARGI#--warp=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI}" = "--just_build" -o "${ARGI}" = "-j" ] ; then
	JUST_MAKE="yes"
    elif [ "${ARGI:0:6}" = "--cool" ] ; then
        COOL_FAC="${ARGI#--cool=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI:0:8}" = "--angle=" ] ; then
	echo "GOT ANGLE"
        DEGREES1="${ARGI#--angle=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI:0:8}" = "--angle1" ] ; then
        echo "GOT ANGLE1"
	DEGREES1="${ARGI#--angle1=*}"
        UNDEFINED_ARG=""
    elif [ "${ARGI:0:8}" = "--angle2" ] ; then
        echo "GOT ANGLE2"
	DEGREES2="${ARGI#--angle2=*}"
        UNDEFINED_ARG="" 
    elif [ "${ARGI:0:5}" = "--key" ] ; then
        KEY="${ARGI#--key=*}"
        UNDEFINED_ARG=""

    elif [ "${ARGI}" = "--adaptive" -o "${ARGI}" = "-a" ] ; then
        ADAPTIVE="true"
        UNDEFINED_ARG=""
    elif [ "${ARGI}" = "--unconcurrent" -o "${ARGI}" = "-uc" ] ; then
        CONCURRENT="false"
        UNDEFINED_ARG=""
    else 
	printf "Bad Argument: %s \n" $ARGI
	exit 0
    fi
done
if [ "${HELP}" = "yes" ]; then
    printf "%s [SWITCHES]            \n" $0
    printf "Switches:                \n"
    printf "  --warp=WARP_VALUE      \n"
    printf "  --adaptive, -a         \n"
    printf "  --unconcurrent, -uc       \n"
    printf "  --angle=DEGREE_VALUE   \n"
    printf "  --angle1=DEGREE_VALUE archie   \n"
    printf "  --angle2=DEGREE_VALUE betty  \n"
    printf "  --cool=COOL_FAC        \n"
    printf "  --just_build, -j       \n"
    printf "  --help, -h             \n"
    exit 0;
fi
#-------------------------------------------------------
#  Part 2: Create the .moos and .bhv files. 
#-------------------------------------------------------

VNAME1="archie"      # The first  vehicle community
START_POS1="0,0"  

VNAME2="betty"
START_POS2="0,0"

nsplug meta_shoreside.moos targ_shoreside.moos -f WARP=$TIME_WARP \
   VNAME="shoreside" SHARE_LISTEN=$SHORE_LISTEN

#start first vehicle:
nsplug meta_vehicle.moos targ_$VNAME1.moos -f WARP=$TIME_WARP  \
   VNAME=$VNAME1      START_POS=$START_POS1                    \
   VPORT="9001"       SHARE_LISTEN="9301"                      \
   VTYPE=UUV          COOL_FAC=$COOL_FAC  COOL_STEPS=$COOL_STEPS\
   CONCURRENT=$CONCURRENT  ADAPTIVE=$ADAPTIVE

nsplug meta_vehicle.bhv targ_$VNAME1.bhv -f VNAME=$VNAME1      \
    START_POS=$START_POS1 SURVEY_X=$SURVEY_X SURVEY_Y=$SURVEY_Y \
        HEIGHT=$HEIGHT1   WIDTH=$WIDTH1 LANE_WIDTH=$LANE_WIDTH1 \
        DEGREES=$DEGREES1

#start second vehicle:                                                                                                   
nsplug meta_vehicle.moos targ_$VNAME2.moos -f WARP=$TIME_WARP  \
   VNAME=$VNAME2      START_POS=$START_POS1                    \
   VPORT="9002"       SHARE_LISTEN="9302"                      \
   VTYPE=UUV          COOL_FAC=$COOL_FAC  COOL_STEPS=$COOL_STEPS\
   CONCURRENT=$CONCURRENT  ADAPTIVE=$ADAPTIVE

nsplug meta_vehicle.bhv targ_$VNAME2.bhv -f VNAME=$VNAME2      \
    START_POS=$START_POS2 SURVEY_X=$SURVEY_X SURVEY_Y=$SURVEY_Y \
        HEIGHT=$HEIGHT2   WIDTH=$WIDTH2 LANE_WIDTH=$LANE_WIDTH2 \
        DEGREES=$DEGREES2


if [ ${JUST_MAKE} = "yes" ] ; then
    exit 0
fi

#-------------------------------------------------------
#  Part 3: Launch the processes
#-------------------------------------------------------
printf "Launching $VNAME1 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$VNAME1.moos >& /dev/null &
sleep .25
printf "Launching $VNAME2 MOOS Community (WARP=%s) \n" $TIME_WARP
pAntler targ_$VNAME2.moos >& /dev/null &
sleep .25
printf "Launching $SNAME MOOS Community (WARP=%s) \n"  $TIME_WARP
pAntler targ_shoreside.moos >& /dev/null &
printf "Done \n"

uMAC targ_shoreside.moos

printf "Killing all processes ... \n"
kill %1 %2 %3
printf "Done killing processes.   \n"


