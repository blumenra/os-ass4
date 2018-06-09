#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{

  if(argc == 4){
	if(strcmp(argv[1], "-s") == 0){
		if(symlink(argv[2], argv[3]) == -1)
			goto failed;
	}
	else{
		goto failed;
	}
	    
	exit();  
	

	failed:
		printf(2, "link -s %s %s: failed\n", argv[2], argv[3]);

	exit();  
  }
  
  if(argc != 3){
    printf(2, "Usage: ln old new\n");
    exit();
  }

  if(link(argv[1], argv[2]) < 0)
    printf(2, "link %s %s: failed\n", argv[1], argv[2]);
  exit();
}
