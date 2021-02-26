PROGRAM program0
  VAR
    LocalVar0 : DINT;
    LocalVar1 : TOD;
  END_VAR

  LocalVar0 := 42;
END_PROGRAM

CONFIGURATION config
  VAR_GLOBAL
    ConfVar0 : DINT;
  END_VAR

  RESOURCE resource1 ON PLC
    (* This leads to FPE in upstream matiec. *)
    TASK task0(INTERVAL := T#0h0m0s0ms,PRIORITY := 1);

    (* Uncomment to build without error: *)
    (* TASK task0(INTERVAL := T#0h0m0s1ms,PRIORITY := 1); *)

    PROGRAM instance0 WITH task0 : program0;
  END_RESOURCE
END_CONFIGURATION

