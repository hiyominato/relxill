#!/bin/csh

if (-d /data/system/software ) then
    setenv SOFTDIR /data/system/software
    setenv RELXILL_TABLE_PATH "/userdata/data/dauser/relline_tables"
else if (-d /home/thomas/software) then
    setenv SOFTDIR /home/thomas/software
    setenv RELXILL_TABLE_PATH "/home/thomas/data/relline_tables"
endif

if (-e $SOFTDIR/softwarescript_Xray.csh ) then
    if (-e $SOFTDIR/softwarescript.csh ) then
	source $SOFTDIR/softwarescript.csh
    endif
    source $SOFTDIR/softwarescript_Xray.csh
    echo " Using scripts $SOFTDIR/softwarescript_Xray.csh "
endif


make
./test_sta

make model
test/check_model_functions.sl
