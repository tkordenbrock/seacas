C Copyright(C) 1999-2020 National Technology & Engineering Solutions
C of Sandia, LLC (NTESS).  Under the terms of Contract DE-NA0003525 with
C NTESS, the U.S. Government retains certain rights in this software.
C
C See packages/seacas/LICENSE for details

C=======================================================================
      LOGICAL FUNCTION LXSET(SET,CH)
      IMPLICIT INTEGER (A-Z)
      CHARACTER*504 ILINE
      COMMON /LXCOM1/ILINE
      COMMON /LXCOM2/JLINE,LXINIT
      CHARACTER*(*) SET,CH

      CH = ILINE(JLINE:JLINE)
      LXSET = INDEX(SET,CH) .NE. 0 .AND. CH .NE. CHAR(0)
      IF (LXSET) THEN
         JLINE = JLINE + 1
      END IF

      RETURN

      END
