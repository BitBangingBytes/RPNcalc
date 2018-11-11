// Main RPN Calculator program written in C.
// Used as a simulator for the Goodwatch RPN calc that will be 
// written in C and add floating point number support
//

#define STACKSIZE 10
#include <stdio.h>

static float stack[STACKSIZE];
static float buffer=0;
static int bufferdirty=0;
static unsigned int stacki=0;

// Peek at top item of the stack
static float rpn_peek(){
  return stack[stacki%STACKSIZE};
}
// Push a new value onto the stack
static void rpn_push(float val){
  stack[(++stacki)%STACKSIZE]=val;
}
// Pop the top item from the stack
static float rpn_pop(){
  long i=stack[(stacki--)%STACKSIZE];
  return i;
}
//! Pushes the buffer to the stack if it has been modified.
static void rpn_pushbuffer(){
  //Do nothing unless the buffer is in use.
  if(!bufferdirty) return;

  //If the buffer is dirty, we'll zero it, mark it clean, and push it
  //to the call stack.
  bufferdirty=0;
  rpn_push(buffer);
  buffer=0;

}
//! Presses one digit into the buffer.
static void rpn_updatebuffer(int i){
  /* Here we mark the buffer as dirty, multiple it by ten, and add the
     incoming digit.  This would of course be more efficient in BCD,
     but GCC doesn't support BCD values and we'd prefer not to write
     in assembly for now.
   */
  bufferdirty=1;
  buffer=buffer*10+i;
}

int main(void) {
    long i, j;
    char c;

  //Operators
  switch(c=getchar()){
  case '=':
    if(bufferdirty)          //Push the value if it's waiting.
      rpn_pushbuffer();
    else                     //Otherwise duplicate bottom stack item.
      rpn_push(rpn_peek());
    break;
  case '.':
    //What should this do?
    //It's our only free button.
    break;
  case '+':
    rpn_pushbuffer();
    rpn_push(rpn_pop()+rpn_pop());
    break;
  case '-':
    rpn_pushbuffer();
    j=rpn_pop();
    i=rpn_pop();
    rpn_push(i-j);
    break;
  case '*':
    rpn_pushbuffer();
    rpn_push(rpn_pop()*rpn_pop());
    break;
  case '/':
    rpn_pushbuffer();
    j=rpn_pop();
    i=rpn_pop();
    rpn_push(i/j);
    break;
  }


  /* Numbers are special.  They modify a buffer, and the buffer is
     pushed onto the stack before an operator or when = is pressed.
  */
  if(c>='0' && c<='9')
    rpn_updatebuffer(c&0xf);

  return 1;//Redraw.
}

