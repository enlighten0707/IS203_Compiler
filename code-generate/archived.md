## Call_class
```cpp
 if(name==print) {
    if (cgen_debug) s<<"<call printf>"<<endl;
     // please set %eax to the number of Float parameters, num.
    //  把%eax赋值为Float类型的参数个数, num
    // emit_sub("$8", RSP, s);
    // emit_irmovl("1", EAX, s);
    // emit_call("printf", s);
    // actuals->nth(0)->code(s);

    emit_push("%rdi",s);
    emit_push("%rsi",s);
    emit_push("%rdx",s);
    emit_push("%rcx",s);
    emit_push("%r8",s);
    emit_push("%r9",s);
    pushFloatRegister(s, "%xmm0");
    pushFloatRegister(s, "%xmm1");
    pushFloatRegister(s, "%xmm2");
    pushFloatRegister(s, "%xmm3");
    pushFloatRegister(s, "%xmm4");
    pushFloatRegister(s, "%xmm5");
    pushFloatRegister(s, "%xmm6");
    pushFloatRegister(s, "%xmm7");

    int fNum=0;
    int notFNum=0;

    for(int i = actuals->first(); actuals->more(i); i = actuals->next(i)) {
      // if (cgen_debug) s << "actual("<<i<<")"<<endl;
      // if (cgen_debug) s << "type: " << actuals->nth(i)->getType() << endl<<endl;
      actuals->nth(i)->code(s);
      // if(actuals->nth(i)->getType()==Float)fNum++;
      if(actuals->nth(i)->getType()==Float) {
        switch (fNum)
        {
        case 0:
          emit_mov("%xmm8","%xmm0",s);
          fNum++;
          break;

        case 1:
          emit_mov("%xmm8","%xmm1",s);
          fNum++;
          break;
        
        case 2:
          emit_mov("%xmm8","%xmm2",s);
          fNum++;
          break;
        
        case 3:
          emit_mov("%xmm8","%xmm3",s);
          fNum++;
          break;
        
        case 4:
          emit_mov("%xmm8","%xmm4",s);
          fNum++;
          break;
        
        case 5:
          emit_mov("%xmm8","%xmm5",s);
          fNum++;
          break;

        case 6:
          emit_mov("%xmm8","%xmm6",s);
          fNum++;
          break;
        
        case 7:
          emit_mov("%xmm8","%xmm7",s);
          fNum++;
          break;

        default:
          emit_sub("$8","%rsp",s);
          emit_mov("%xmm8",generateParam(0,"%rsp"),s);
          fNum++;
          break;
        }
      } 
      else {
        switch (notFNum)
        {
        case 0:
          emit_mov("%rax","%rdi",s);
          notFNum++;
          break;

        case 1:
          emit_mov("%rax","%rsi",s);
          notFNum++;
          break;
        
        case 2:
          emit_mov("%rax","%rdx",s);
          notFNum++;
          break;
        
        case 3:
          emit_mov("%rax","%rcx",s);
          notFNum++;
          break;
        
        case 4:
          emit_mov("%rax","%r8",s);
          notFNum++;
          break;
        
        case 5:
          emit_mov("%rax","%r9",s);
          notFNum++;
          break;

        default:
          emit_sub("$8","%rsp",s);
          emit_mov("%rax",generateParam(0,"%rsp"),s);
          notFNum++;
          break;
        }
      }
    }

    if(flag) emit_sub("$8","%rsp",s);
    if(fNum) {
      emit_irmovl(generateInt(fNum), EAX, s);
    } 
    else {
      emit_irmovl("0", EAX, s);
    } 
    emit_call(name->get_string(),s);
    if(flag) emit_add("$8","%rsp",s);


    popFloatRegister(s, "%xmm7");
    popFloatRegister(s, "%xmm6");
    popFloatRegister(s, "%xmm5");
    popFloatRegister(s, "%xmm4");
    popFloatRegister(s, "%xmm3");
    popFloatRegister(s, "%xmm2");
    popFloatRegister(s, "%xmm1");
    popFloatRegister(s, "%xmm0");
    emit_pop("%r9",s);
    emit_pop("%r8",s);
    emit_pop("%rcx",s);
    emit_pop("%rdx",s);
    emit_pop("%rsi",s);
    emit_pop("%rdi",s);

    if (cgen_debug) s<<"</call printf>"<<endl;

  } 
  
  else {
    emit_push("%rdi",s);
    emit_push("%rsi",s);
    emit_push("%rdx",s);
    emit_push("%rcx",s);
    emit_push("%r8",s);
    emit_push("%r9",s);
    pushFloatRegister(s, "%xmm0");
    pushFloatRegister(s, "%xmm1");
    pushFloatRegister(s, "%xmm2");
    pushFloatRegister(s, "%xmm3");
    pushFloatRegister(s, "%xmm4");
    pushFloatRegister(s, "%xmm5");
    pushFloatRegister(s, "%xmm6");
    pushFloatRegister(s, "%xmm7");

    int fNum=0;
    int notFNum=0;

    for(int i = actuals->first(); actuals->more(i); i = actuals->next(i)) {
      // if (cgen_debug) s << "actual("<<i<<")"<<endl;
      // if (cgen_debug) s << "type: " << actuals->nth(i)->getType() << endl<<endl;
      actuals->nth(i)->code(s);
      if(actuals->nth(i)->getType()==Float) {
        switch (fNum)
        {
        case 0:
          emit_mov("%xmm8","%xmm0",s);
          fNum++;
          break;

        case 1:
          emit_mov("%xmm8","%xmm1",s);
          fNum++;
          break;
        
        case 2:
          emit_mov("%xmm8","%xmm2",s);
          fNum++;
          break;
        
        case 3:
          emit_mov("%xmm8","%xmm3",s);
          fNum++;
          break;
        
        case 4:
          emit_mov("%xmm8","%xmm4",s);
          fNum++;
          break;
        
        case 5:
          emit_mov("%xmm8","%xmm5",s);
          fNum++;
          break;

        case 6:
          emit_mov("%xmm8","%xmm6",s);
          fNum++;
          break;
        
        case 7:
          emit_mov("%xmm8","%xmm7",s);
          fNum++;
          break;

        default:
          emit_sub("$8","%rsp",s);
          emit_mov("%xmm8",generateParam(0,"%rsp"),s);
          fNum++;
          break;
        }
      } else {
        switch (notFNum)
        {
        case 0:
          emit_mov("%rax","%rdi",s);
          notFNum++;
          break;

        case 1:
          emit_mov("%rax","%rsi",s);
          notFNum++;
          break;
        
        case 2:
          emit_mov("%rax","%rdx",s);
          notFNum++;
          break;
        
        case 3:
          emit_mov("%rax","%rcx",s);
          notFNum++;
          break;
        
        case 4:
          emit_mov("%rax","%r8",s);
          notFNum++;
          break;
        
        case 5:
          emit_mov("%rax","%r9",s);
          notFNum++;
          break;

        default:
          emit_sub("$8","%rsp",s);
          emit_mov("%rax",generateParam(0,"%rsp"),s);
          notFNum++;
          break;
        }
      }
    }

    emit_call(name->get_string(),s);


    popFloatRegister(s, "%xmm7");
    popFloatRegister(s, "%xmm6");
    popFloatRegister(s, "%xmm5");
    popFloatRegister(s, "%xmm4");
    popFloatRegister(s, "%xmm3");
    popFloatRegister(s, "%xmm2");
    popFloatRegister(s, "%xmm1");
    popFloatRegister(s, "%xmm0");
    emit_pop("%r9",s);
    emit_pop("%r8",s);
    emit_pop("%rcx",s);
    emit_pop("%rdx",s);
    emit_pop("%rsi",s);
    emit_pop("%rdi",s);
  }
  
```