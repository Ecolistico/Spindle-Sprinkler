/*     Secuencia para hacer aspersores */
/*     Definicion relevadores */
const int spindle1 = A14; //Spindle central
const int spindle2 = A13; // Spindle derecho
const int spindle3 = A12; // Spindle izquierdo

/* Definir pines para mover spindle central */
const int stepPin1 = 33; 
const int dirPin1 = 35;
const int enable1 = 37;
const int finish_button1 = 12;

/* Definir pines para mover spindle derecho */
const int stepPin2 = 39;
const int dirPin2 = 41;
const int enable2 = 43;
const int finish_button2 = 13;

/* Definir pines para mover spindle izquierdo */
const int stepPin3 = 45;
const int dirPin3 = 47;
const int enable3 = 49;
const int finish_button3 = 11;

/* Extrusor de tubo*/
const int stepPinT = 27;
const int dirPinT = 29;
const int enableT = 31;

/* Botón para activar la secuencia de perforador*/
const int LimitSwitch = 10;

/* Variables auxiliares */
int aux=0;
int central_pos=0;
int left_pos=0;
int aux_left;
int right_pos=0;
int aux_right;
const int CentralSteps = 9400;
const int LeftSteps = 14100; //  Looking at the machine from behind
const int RightSteps = 15800; //

void setup() {
/* Definir todos los pines como salidas o entradas */
  Serial.begin(9600);
  pinMode(stepPin1,OUTPUT); 
  pinMode(dirPin1,OUTPUT);
  pinMode(enable1,OUTPUT);
  pinMode(spindle1,OUTPUT);
  
  pinMode(stepPin2,OUTPUT); 
  pinMode(dirPin2,OUTPUT);
  pinMode(enable2,OUTPUT);
  pinMode(spindle2,OUTPUT);
  
  pinMode(stepPin3,OUTPUT); 
  pinMode(dirPin3,OUTPUT);
  pinMode(enable3,OUTPUT);
  pinMode(spindle3,OUTPUT);
  
  pinMode(stepPinT,OUTPUT); 
  pinMode(dirPinT,OUTPUT);
  pinMode(enableT,OUTPUT);
  
  pinMode(LimitSwitch, INPUT_PULLUP);
  pinMode(finish_button1, INPUT_PULLUP);
  pinMode(finish_button2, INPUT_PULLUP);
  pinMode(finish_button3, INPUT_PULLUP);

/* Desactivar Nema17 */ 
  digitalWrite(enable1,HIGH);
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3,HIGH);
  digitalWrite(enableT,HIGH);
    
/* Desactivar Spindles */ 
  digitalWrite(spindle1,LOW);
  digitalWrite(spindle2,LOW);
  digitalWrite(spindle3,LOW);

  /*Extruir tubo*/
  digitalWrite(enableT,LOW);
  digitalWrite(dirPinT,HIGH);
  while (digitalRead(LimitSwitch)==HIGH){
    digitalWrite(stepPinT,HIGH); 
    delayMicroseconds(1000); 
    digitalWrite(stepPinT,LOW); 
    delayMicroseconds(1000);
    aux=1;
  }
  /****************** Secuencia de calibración ****************************/
  digitalWrite(enable1,LOW);
  digitalWrite(enable2, LOW);
  digitalWrite(enable3,LOW);
  digitalWrite(dirPin1,LOW);
  digitalWrite(dirPin2,LOW);
  digitalWrite(dirPin3,LOW);

  bool setup_motors=false;
  
  while(setup_motors==false){
    
   if(digitalRead(finish_button1)==HIGH){
    digitalWrite(stepPin1,HIGH); 
    delayMicroseconds(75); 
    digitalWrite(stepPin1,LOW); 
    delayMicroseconds(75);
   }
   
   if(digitalRead(finish_button2)==HIGH){
    digitalWrite(stepPin2,HIGH); 
    delayMicroseconds(75); 
    digitalWrite(stepPin2,LOW); 
    delayMicroseconds(75);
   }
   
   if(digitalRead(finish_button3)==HIGH){
    digitalWrite(stepPin3,HIGH); 
    delayMicroseconds(75); 
    digitalWrite(stepPin3,LOW); 
    delayMicroseconds(75);
   }
   
   if(digitalRead(finish_button1)==LOW && digitalRead(finish_button2)==LOW && digitalRead(finish_button3)==LOW){
    setup_motors=true;  
   }
   
  }
 
  digitalWrite(dirPin1,HIGH);
  digitalWrite(dirPin2,HIGH);
  digitalWrite(dirPin3,HIGH);

  for(int x = 0; x < 1600; x++) {
      digitalWrite(stepPin1,HIGH); 
      digitalWrite(stepPin2,HIGH); 
      digitalWrite(stepPin3,HIGH); 
      delayMicroseconds(100); 
      digitalWrite(stepPin1,LOW);
      digitalWrite(stepPin2,LOW); 
      digitalWrite(stepPin3,LOW); 
      delayMicroseconds(100); 
    }
    
  digitalWrite(enable1,HIGH);
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3,HIGH);
  /****************** Secuencia de calibración ****************************/  
}


void loop() {

/* Esperar a presionar botón para hacer aspersor */
if (digitalRead(LimitSwitch)==HIGH){
    digitalWrite(stepPinT,HIGH); 
    delayMicroseconds(1500); 
    digitalWrite(stepPinT,LOW); 
    delayMicroseconds(1500);
}
if(digitalRead(LimitSwitch)==LOW && aux==0){
  int TubeSteps=0;
  while (TubeSteps<1157/14*22.5){
    digitalWrite(stepPinT,HIGH); 
    delayMicroseconds(1500); 
    digitalWrite(stepPinT,LOW); 
    delayMicroseconds(1500);
    TubeSteps++;
    aux=1;
  }
}
if(digitalRead(LimitSwitch)==HIGH){
  aux=0;
}
if(aux==1){
  
  /************************ Secuencia para de perforación ***********************************/
   int vel1=100;

   digitalWrite(enable1,LOW);
   digitalWrite(dirPin1,LOW);
   while(digitalRead(finish_button1)==HIGH){
      digitalWrite(stepPin1,HIGH); 
      delayMicroseconds(vel1); 
      digitalWrite(stepPin1,LOW); 
      delayMicroseconds(vel1); 
   }
   
   digitalWrite(dirPin1,HIGH);
   for(central_pos=0; central_pos<CentralSteps; central_pos++) {
     if(central_pos>=0 && central_pos<CentralSteps-6000){
      vel1=100;
     }
     if(central_pos>=6000 && central_pos<CentralSteps-1500){
      vel1=250;
     }
     if(central_pos>=CentralSteps-1500){
      vel1=1000;
     }
     if(central_pos>=CentralSteps-800){
      vel1=3000;
     }
     if(central_pos==CentralSteps-1500){
      digitalWrite(spindle1,HIGH); // Conectado a relevador con lógica inversa
     }
     
     digitalWrite(stepPin1,HIGH); 
     delayMicroseconds(vel1); 
     digitalWrite(stepPin1,LOW); 
     delayMicroseconds(vel1); 
   }
   
   digitalWrite(enable2,LOW);    
   digitalWrite(dirPin1,LOW); 
   digitalWrite(dirPin2,LOW);
   while (central_pos>=CentralSteps-800){
     digitalWrite(stepPin1,HIGH); 
     delayMicroseconds(vel1); 
     digitalWrite(stepPin1,LOW); 
     delayMicroseconds(vel1); 
     central_pos--;
   }
   vel1=100;
   int vel2=100;
   
   for(central_pos; central_pos>2500; central_pos--) {
     if(central_pos<CentralSteps-2000){
      digitalWrite(spindle1,LOW); // Desactivar Spindle
     }
     digitalWrite(stepPin1,HIGH);
     delayMicroseconds(vel1);
     digitalWrite(stepPin1,LOW);
     delayMicroseconds(vel1);
     
     if(digitalRead(finish_button2)==LOW){
        digitalWrite(dirPin2,HIGH);
        left_pos=0;
        aux_left=1;
     }
     
     digitalWrite(stepPin2,HIGH);
     delayMicroseconds(vel2);
     digitalWrite(stepPin2,LOW);
     delayMicroseconds(vel2);

     if(digitalRead(dirPin2)==HIGH){
      left_pos++;
     }
   }
   digitalWrite(enable1,HIGH);
   /*if(aux_left!=1){
    vel2 = 100;
    while(digitalRead(finish_button2)==HIGH){
      digitalWrite(stepPin2,HIGH); 
      delayMicroseconds(vel2); 
      digitalWrite(stepPin2,LOW); 
      delayMicroseconds(vel2); 
    }
    digitalWrite(dirPin2,HIGH);
    left_pos=0;
   }*/

   for(left_pos; left_pos<LeftSteps ; left_pos++){
    if(left_pos<=LeftSteps-3000){
      vel2=100;
    }
    if(left_pos>LeftSteps-3000 && left_pos<LeftSteps-500){
      vel2=250;
    }
    if(left_pos>=LeftSteps-1500){
      vel2=500;
    }
    if(left_pos>=LeftSteps-500){
      vel2=1000;
    }
    if(left_pos==LeftSteps-7500){
      digitalWrite(spindle2,HIGH); 
    }
    digitalWrite(stepPin2,HIGH); 
    delayMicroseconds(vel2); 
    digitalWrite(stepPin2,LOW); 
    delayMicroseconds(vel2); 
   }
  digitalWrite(dirPin2,LOW); 
  digitalWrite(dirPin3,LOW);
  vel2 = 75;
  int vel3=75;
  for(left_pos; left_pos>2000 ; left_pos--){
    if(left_pos==LeftSteps-3000){
      digitalWrite(spindle2,LOW); // Desactivar spindle
    }
    
    digitalWrite(stepPin2,HIGH);
    delayMicroseconds(vel2);
    digitalWrite(stepPin2,LOW);
    delayMicroseconds(vel2);
    
    digitalWrite(enable3,LOW);
    if(digitalRead(finish_button3)==LOW){
        digitalWrite(dirPin3,HIGH);
        right_pos=0;
        aux_right=1;
     }
     if (right_pos<7000){
      digitalWrite(stepPin3,HIGH);
      delayMicroseconds(vel3);
      digitalWrite(stepPin3,LOW);
      delayMicroseconds(vel3);

      if(digitalRead(dirPin3)==HIGH){
        right_pos++;
      }
     }
    
  }
  
   digitalWrite(enable2,HIGH);
   /*if(aux_right!=1){
    vel3 = 75;
    while(digitalRead(finish_button3)==HIGH){
      digitalWrite(stepPin3,HIGH); 
      delayMicroseconds(vel3); 
      digitalWrite(stepPin3,LOW); 
      delayMicroseconds(vel3); 
    }
    digitalWrite(dirPin3,HIGH);
    right_pos=0;
   }*/

  for(right_pos; right_pos<RightSteps;right_pos++){
    if(right_pos<=RightSteps-3000){
      vel3=100; 
    }
    if(right_pos==RightSteps-7500){
      digitalWrite(spindle3,HIGH); 
    }
    if( (right_pos>RightSteps-3000 && right_pos<=RightSteps-500) ){
      vel3=250;
    }
    if(right_pos>=RightSteps-1500){
      vel3=500;
    }
    if(right_pos>=RightSteps-500){
      vel3=1000;
    }
    digitalWrite(stepPin3,HIGH); 
    delayMicroseconds(vel3); 
    digitalWrite(stepPin3,LOW); 
    delayMicroseconds(vel3); 
  }
   
  digitalWrite(dirPin3,LOW);
  vel3=75;
  for(right_pos; right_pos>3000 ; right_pos--){
    if(right_pos==RightSteps-7000){
      digitalWrite(spindle3,LOW); // Desactivar spindle
    }
   digitalWrite(stepPin3,HIGH);
   delayMicroseconds(vel3);
   digitalWrite(stepPin3,LOW);
   delayMicroseconds(vel3);
  }
   aux=0;
   digitalWrite(enable3,HIGH);
 
  }
}
