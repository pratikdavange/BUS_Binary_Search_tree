#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>

typedef struct BinarySearchTree BST;

struct BinarySearchTree
{
  int PassnNo; // busNo0SeatNo.
  char name[10];
  int payment;
  struct BinarySearchTree *left;
  struct BinarySearchTree *right;
};

BST *root = NULL;
int cost(BST *r);
void status();
void busLists();
void DisplaySeat(int bus[33]);
void cancel(int x);
BST *reservationInfo(BST *, int, int *);
BST *insert(BST **r, int custID);

int busSeat[32][9] = {0};

void redColor()
{
  printf("\033[1;31m");
}

void resetColor()
{
  printf("\033[0m");
}

BST *reservationInfo(BST *r, int s, int *custIDmatched)
{
  if (r == NULL)
    return NULL;
  BST *presentnode = r;
  while (presentnode)
  {
    if (presentnode->PassnNo == s)
    {
      *custIDmatched = 1;
      redColor();
      printf("\n-----------------------------------------------------------------");
      printf("\n||              NAME: %10s                               ||", (presentnode->name));
      printf("\n||              CUSTOMER ID: %d                              ||", presentnode->PassnNo);
      printf("\n||              BUS NUMBER: %d                                  ||", (presentnode->PassnNo) / 1000);
      printf("\n||              SEAT NUMBER: %d                                 ||", (presentnode->PassnNo) % 100);
      printf("\n||              TICKET COST: Rs.%d                             ||", cost(presentnode));
      printf("\n-----------------------------------------------------------------");
      resetColor();
      getch();
      return r;
    }
    else if (presentnode->PassnNo > s)
      presentnode = presentnode->left;
    else
      presentnode = presentnode->right;
  }

  return NULL;
}

BST *insert(BST **r, int custId)
{
  if (*r == NULL)
  {

    *r = (BST *)malloc(sizeof(BST));
    (*r)->PassnNo = custId;
    if (*r == NULL)
    {
      printf("No memory…");
      return NULL;
    }
    else
    {
      (*r)->left = (*r)->right = NULL;
      printf("\n   ENTER THE PERSON NAME: ");
      // fflush(cbin);
      scanf("%s", &((*r)->name));
      printf("\n   ENTER THE TICKET COST: ");
      fflush(cbin);
      scanf("%d", &((*r)->payment));

      FILE *fp;
      fp = fopen("bookings.csv", "a");
      fprintf(fp, "Cust_ID\n%d,No_of_seats\n%d,SeatNo\n%d,Name\n%s,Payment\n%d", (*r)->PassnNo, (*r)->PassnNo/1000, (*r)->PassnNo%100, (*r)->name, (*r)->payment);
      fclose(fp);
    }
  }
  else
  {
    if ((*r)->PassnNo > custId)
    {
      (*r)->left = insert(&((*r)->left), custId);
    }
    else if ((*r)->PassnNo < custId)
    {
      (*r)->right = insert(&((*r)->right), custId);
    }
  }
  return *r;
}


void DisplaySeat(int bus[33])
{
printf("\n");
for (int i = 1; i <= 32; i++)
{
printf(" ");
if (i == 1 || i == 10 || i == 11 || i == 20)
printf("\n");
if (bus[i] == 0)
  printf("%d:EMPTY\t", i);
else
  printf("%d:FULL\t", i);
}
printf("\n\n");
}

void busLists()
{
  int bus;
  printf("\n\n   CHOOSE BUS NUMBER : \n");
  printf("   ------------------\n");
  printf("   |    1. BUS 1    |\n");
  printf("   |    2. BUS 2    |\n");
  printf("   |    3. BUS 3    |\n");
  printf("   |    4. BUS 4    |\n");
  printf("   |    5. BUS 5    |\n");
  printf("   ------------------\n");

  printf("\n   ENTER BUS NUMBER: ");
  scanf("%d", &bus);

  if (bus > 5 || bus < 1)
  {
    redColor();
    printf("\n\n   PLEASE ENTER A VALID BUS NUMBER\n");
    resetColor();
    getch();
    return;
  }

  printf("\n\n   AVAILABLE SEATS FOR BUS %d: \n", bus);
  printf("   --------------------------------\n");
  printf("   |   1  2  3  4  5  6  7  8  9  |\n");
  printf("   --------------------------------\n");

  for (int i = 0; i < 32; i++)
  {
    printf("   | %2d", i + 1);
    for (int j = 0; j < 9; j++)
    {
      if (busSeat[i][j] == bus)
      {
        printf(" X ");
      }
      else
      {
        printf(" - ");
      }
    }
    printf("|\n");
  }
  printf("   --------------------------------\n");

  printf("\n   PRESS ANY KEY TO CONTINUE...");
  getch();
}

void cancel(int x)
{
  int match = 0;
  root = reservationInfo(root, x, &match);
  if (match == 0)
  {
    redColor();
    printf("\n   NO RECORDS FOUND FOR THE GIVEN CUSTOMER ID\n");
    resetColor();
    getch();
    return;
  }
  else
  {
    BST *prevnode = NULL, *presentnode = NULL;
    presentnode = root;

    while (presentnode && presentnode->PassnNo != x)
    {
      prevnode = presentnode;
      if (presentnode->PassnNo > x)
        presentnode = presentnode->left;
      else
        presentnode = presentnode->right;
    }
    if (prevnode == NULL)
    {
      if (presentnode->left == NULL && presentnode->right == NULL)
        root = NULL;
      else if (presentnode->left != NULL && presentnode->right == NULL)
        root = presentnode->left;
      else if (presentnode->left == NULL && presentnode->right != NULL)
        root = presentnode->right;
      else
      {
        BST *prev2node = presentnode->right;
        while (prev2node->left)
        {
          prev2node = prev2node->left;
        }
        prev2node->left = presentnode->left;
        root = presentnode->right;
      }
    }
    else
    {
      if (presentnode->left == NULL && presentnode->right == NULL)
      {
        if (prevnode->left == presentnode)
          prevnode->left = NULL;
        else
          prevnode->right = NULL;
      }
      else if (presentnode->left != NULL && presentnode->right == NULL)
      {
        {
if (prevnode->left == presentnode)
prevnode->left = presentnode->left;
else
prevnode->right = presentnode->left;
}
else 
if (presentnode->left == NULL && presentnode->right != NULL)
{
if (prevnode->left == presentnode)
prevnode->left = presentnode->right;
else
prevnode->right = presentnode->right;
}
else
{
BST *leftmost = presentnode->right;
while (leftmost->left != NULL)
{
prevnode = leftmost;
leftmost = leftmost->left;
}
presentnode->PassnNo = leftmost->PassnNo;
strcpy(presentnode->name, leftmost->name);
presentnode->payment = leftmost->payment;
if (prevnode->left == leftmost)
prevnode->left = NULL;
else
prevnode->right = NULL;
free(leftmost);
}
printf("\n CANCELLATION SUCCESSFULLY DONE! PRESS ANY KEY TO CONTINUE...");
getch();
return;
}
prevnode = presentnode;
if (presentnode->PassnNo > s)
presentnode = presentnode->left;
else
presentnode = presentnode->right;
}
printf("\n CUSTOMER ID NOT FOUND! PRESS ANY KEY TO CONTINUE...");
getch();
}


void status()
{
  int custIDmatched = 0, id;

  printf("\nEnter customer id: ");
  scanf("%d", &id);

  reservationInfo(root, id, &custIDmatched);

  if (custIDmatched == 0)
  {
    printf("\nNo such customer id found in our record");
    getch();
  }
}


int cost(BST *r)
{
  int busno = r->PassnNo / 1000;
  int seatno = r->PassnNo % 100;
  if (seatno <= 2)
  {
    return 250;
  }
  else if (seatno >= 3 && seatno <= 5)
  {
    return 200;
  }
  else
  {
    return 150;
  }
}

void cancel(int x)
{
  int seatNo = x % 100;
  int busNo = x / 1000;
  BST *temp, *parent, *xptr;
  temp = root;
  while (temp != NULL)
  {
    if (temp->PassnNo == x)
      break;
    parent = temp;
    if (temp->PassnNo > x)
      temp = temp->left;
    else
      temp = temp->right;
  }
  if (temp == NULL)
  {
    printf("No such passenger\n");
    return;
  }
  if (temp->left == NULL && temp->right == NULL)
  {
    if (temp == root)
    {
      free(root);
      root = NULL;
    }
    else if (parent->left == temp)
    {
      free(temp);
      parent->left = NULL;
    }
    else
    {
      free(temp);
      parent->right = NULL;
    }
  }
  else if (temp->left == NULL || temp->right == NULL)
  {
    if (temp->left == NULL)
      xptr = temp->right;
    else
      xptr = temp->left;
    if (temp == root)
    {
      root = xptr;
      free(temp);
    }
    else if (temp == parent->left)
    {
      free(temp);
      parent->left = xptr;
    }
    else
    {
      free(temp);
      parent->right = xptr;
    }
  }
  else
  {
    BST *p;
    xptr = temp->left;
    if (xptr->right == NULL)
    {
      temp->PassnNo = xptr->PassnNo;
      free(xptr);
      temp->left = NULL;
    }
    else
    {
      while (xptr->right != NULL)
      {
        p = xptr;
        xptr = xptr->right;
      }
      temp->PassnNo = xptr->PassnNo;
      free(xptr);
      p->right = NULL;
    }
  }
  printf("\n\n Seat number %d in Bus %d has been cancelled\n", seatNo, busNo);
  busSeat[busNo][seatNo] = 0;
  getch();
}

int main()
{
  int choice, seatNum, custIDmatched;
  root = NULL;

  while (1)
  {
    system("cls");
    printf("\n\n=================================================================");
    printf("\n                 BUS RESERVATION SYSTEM                           ");
    printf("\n=================================================================");
    printf("\n\n");
    printf("\n[1] RESERVE A SEAT");
    printf("\n[2] CANCEL A RESERVATION");
    printf("\n[3] DISPLAY BUS STATUS");
    printf("\n[4] DISPLAY BUS LISTS");
    printf("\n[5] EXIT");
    printf("\n\nENTER YOUR CHOICE: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      system("cls");
      status();
      printf("\n\nENTER THE SEAT NUMBER: ");
      scanf("%d", &seatNum);

      if (seatNum >= 1 && seatNum <= 288)
      {
        if (busSeat[seatNum / 9][seatNum % 9] == 0)
        {
          busSeat[seatNum / 9][seatNum % 9] = 1;

          int busNo = (seatNum / 9) + 1;
          int seatNo = seatNum % 9;

          int custID = busNo * 1000 + seatNo;

          root = insert(&root, custID);
          printf("\n   SEAT RESERVED SUCCESSFULLY!");
        }
        else
        {
          redColor();
          printf("\n   THE SEAT IS ALREADY RESERVED!\n   CHOOSE ANOTHER SEAT...");
          resetColor();
          getch();
        }
      }
      else
      {
        redColor();
        printf("\n   INVALID SEAT NUMBER!\n   PLEASE ENTER A SEAT NUMBER BETWEEN 1 AND 288...");
        resetColor();
        getch();
      }
      break;

    case 2:
      system("cls");
      printf("\nENTER THE RESERVATION ID: ");
      scanf("%d", &seatNum);
      custIDmatched = 0;
      root = reservationInfo(root, seatNum, &custIDmatched);
      if (custIDmatched)
      {
        printf("\nDO YOU WANT TO CANCEL THIS RESERVATION? (Y/N): ");
        char c = getch();
        if (c == 'Y' || c == 'y')
        {
          cancel(seatNum);
          printf("\n   RESERVATION CANCELLED SUCCESSFULLY!");
        }
        else
        {
          printf("\n   RESERVATION CANCELLED...");
        }
        getch();
      }
      else
      {
        redColor();
        printf("\n   RESERVATION ID NOT FOUND!");
        resetColor();
        getch();
      }
      break;

    case 3:
      system("cls");
      status();
      printf("\n\n");
      DisplaySeat((int *)busSeat);
      getch();
      break;

    case 4:
      system("cls");
      busLists();
      getch();
      break;

    case 5:
      exit(0);
      break;

    default:
      redColor();
      printf("\nINVALID CHOICE!");
      resetColor();
      getch();
    }
  }

  return 0;
}
