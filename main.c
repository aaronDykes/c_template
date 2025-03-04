#include <stdio.h>
#include "stack.h"
#include "vector.h"
#include "table.h"
#include <string.h>


int main (int argc, char **argv)
{

      table *t = NULL;

      _key *k = NULL;

      t = GROW_TABLE(NULL, INIT_SIZE);

      const char *str = "Hello kitty";

      k = Key(str, strlen(str) + 1);

      element e = Num(69420.69);

      write_table(t, k, e);

      print(find_entry(&t, k));

      e = GEN(t, T_TABLE);

      free_obj(&e);





      return 0;

}
