$ IF P1 .EQS. "" THEN INQUIRE P1 "Terminal Device"
$ IF P2 .EQS. "" THEN INQUIRE P2 "Hardcopy Device"
$ TERM = P1
$ HARD = P2
$!
$ TMPFILE = "0GEN3D.TMP"
$ GENDIR := [apgilke.gen3d]
$ IN2D := EXAMPLE
$ T3D := TRAN3
$ R3D := ROT3
$ RC3D := ROTC3
$!
$ OPEN/WRITE OUTFILE 'TMPFILE'
$ WRITE OUTFILE "DETOUR"
$ WRITE OUTFILE "QA OFF"
$ WRITE OUTFILE "LEGEND OFF"
$ WRITE OUTFILE "AXIS OFF"
$ WRITE OUTFILE "CAPTION"
$ WRITE OUTFILE "FIGURE 1 - 2D MESH INPUT TO GEN3D"
$ WRITE OUTFILE ""
$ WRITE OUTFILE ""
$ IF TERM .NES. "" THEN WRITE OUTFILE "PLOT"
$ IF HARD .NES. "" THEN WRITE OUTFILE "HARDCOPY"
$ WRITE OUTFILE "EXIT"
$ CLOSE OUTFILE
$ BLOT 'gendir''IN2D'.GEN "''TERM'" "''HARD'" 'TMPFILE'
$ IF HARD .NES. "" THEN RENAME 'IN2D'.'HARD' GEN3DFIG1.'HARD'
$ DELETE 'TMPFILE';*
$!
$ OPEN/WRITE OUTFILE 'TMPFILE'
$ WRITE OUTFILE "DETOUR"
$ WRITE OUTFILE "QA OFF"
$ WRITE OUTFILE "LEGEND OFF"
$ WRITE OUTFILE "AXIS OFF"
$ WRITE OUTFILE "CAPTION"
$ WRITE OUTFILE "FIGURE 2 - TRANSLATION (10 levels)"
$ WRITE OUTFILE ""
$ WRITE OUTFILE ""
$ WRITE OUTFILE "ROTATE X 60 Y 10"
$ IF TERM .NES. "" WRITE OUTFILE "PLOT"
$ IF HARD .NES. "" THEN WRITE OUTFILE "HARDCOPY"
$ WRITE OUTFILE "EXIT"
$ CLOSE OUTFILE
$ BLOT 'gendir''T3D'.GEN "''TERM'" "''HARD'" 'TMPFILE'
$ IF HARD .NES. "" THEN RENAME 'T3D'.'HARD' GEN3DFIG2.'HARD'
$ DELETE 'TMPFILE';*
$!
$ OPEN/WRITE OUTFILE 'TMPFILE'
$ WRITE OUTFILE "DETOUR"
$ WRITE OUTFILE "QA OFF"
$ WRITE OUTFILE "LEGEND OFF"
$ WRITE OUTFILE "AXIS OFF"
$ WRITE OUTFILE "CAPTION"
$ WRITE OUTFILE "FIGURE 3 - ROTATION AROUND LINE OUTSIDE MESH"
$ WRITE OUTFILE "(9 levels for a total of 270 degrees)"
$ WRITE OUTFILE ""
$ WRITE OUTFILE "ROTATE X 60"
$ IF TERM .NES. "" WRITE OUTFILE "PLOT"
$ IF HARD .NES. "" THEN WRITE OUTFILE "HARDCOPY"
$ WRITE OUTFILE "EXIT"
$ CLOSE OUTFILE
$ BLOT 'gendir''R3D'.GEN "''TERM'" "''HARD'" 'TMPFILE'
$ IF HARD .NES. "" THEN RENAME 'R3D'.'HARD' GEN3DFIG3.'HARD'
$ DELETE 'TMPFILE';*
$!
$ OPEN/WRITE OUTFILE 'TMPFILE'
$ WRITE OUTFILE "DETOUR"
$ WRITE OUTFILE "QA OFF"
$ WRITE OUTFILE "LEGEND OFF"
$ WRITE OUTFILE "AXIS OFF"
$ WRITE OUTFILE "CAPTION"
$ WRITE OUTFILE "FIGURE 4 - ROTATION AROUND MESH EDGE"
$ WRITE OUTFILE "(40 levels for a total of 360 degrees)"
$ WRITE OUTFILE ""
$ WRITE OUTFILE "ROTATE X 60"
$ IF TERM .NES. "" WRITE OUTFILE "PLOT"
$ IF HARD .NES. "" THEN WRITE OUTFILE "HARDCOPY"
$ WRITE OUTFILE "EXIT"
$ CLOSE OUTFILE
$ BLOT 'gendir''RC3D'.GEN "''TERM'" "''HARD'" 'TMPFILE'
$ IF HARD .NES. "" THEN RENAME 'RC3D'.'HARD' GEN3DFIG4.'HARD'
$ DELETE 'TMPFILE';*
$!
$ IF HARD .NES. "" THEN PURGE GEN3DFIG*.'HARD'
