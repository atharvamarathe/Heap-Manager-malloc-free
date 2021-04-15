project : main.c memoryallocator.c sizeclasses.c sizeclassfreelist.c largealloc.c freelist.c
			cc main.c memoryallocator.c sizeclasses.c sizeclassfreelist.c largealloc.c freelist.c -Wall -o project

clean :
		rm  project

