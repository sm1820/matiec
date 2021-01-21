PROGRAM program0
  VAR
    LocalVar0 : DINT;
  END_VAR
  LocalVar0 := 42;
END_PROGRAM

CONFIGURATION config
  RESOURCE resource1 ON PLC
    VAR_GLOBAL
      ResVar0 : DINT;
      ResVar1 : BOOL;
    END_VAR
    TASK task0(INTERVAL := T#20ms,PRIORITY := 0);
    PROGRAM instance0 WITH task0 : program0;
  END_RESOURCE
(* END_CONFIGURATION *)
