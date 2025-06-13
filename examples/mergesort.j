type NumArray = [Int];

func merge(val arr : NumArray, val left : Int, val mid : Int, val right : Int) : Void{

	val x1 : Int = mid - left + 1;
	val x2 : Int = right - mid + 1;

	val leftArray : NumArray = arr[0] * x1;
	val rightArray : NumArray = arr[0] * x2;
	
	for (val i : Int = 0; i < x1; i = i + 1){
		leftArray[i] = arr[left + i];
	}

	for (val j : Int = 0; i < x2; i = i + 1){
		rightArray[i] = arr[mid + i + j];
	}

	val i : Int = 0;
	val j : Int = 0;
	val k : Int = left;

	while (i < x1 && j < x2){
		if (leftArray[i] <= rightArray[j]){
			arr[k] = leftArray[i];
			i = i + 1;
		} else {
			arr[k] = leftArray[i];
			j = j + 1;
		}
		k = k + 1;
	}
}

func mergeSort (val arr : NumArray, val left : Int, val right : Int) : Void{ 
	if (left < right) {
		val mid : Int = left + (right - left) / 2;

		mergeSort(arr, left, mid);
		mergeSort(arr, mid + 1, right);

		merge(arr, left, mid, right);
	}
}

func printArray(val arr : NumArray) : Void{ 
	print("[ ");
   	for (val i : Int = 0; i < arr.length; i = i + 1) {  
print(arr[i]);
if (i < arr.length - 1){
	print(", ");
	}
}
print(" ]");
}

func main () : Void { 
	val numbers : NumArray = [2 ,5, 8, 4, 3, 9, 1, 6];

	print("Original array");
	printArray(number);

	mergeSort(numbers, 0, numbers.length - 1);

	print("Sorted array:");
	printArray(numbers);
}
