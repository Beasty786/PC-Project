import random

def generate(n):
	n = int(input())
	n = pow(2,n) 
	array = [0]*n
	t = int(n/2)
	print(n)
	for i in range(0,t):
		start = i*n
		end = (i+1)*n
		rand = random.randint(start , end)
		if(i>0 and array[i-1] == rand):
			rand = rand +1
		
		array[i] = rand
		array[-i-1] = rand-1
	return array
	
def printArr(array):
	string = ''
	for i in array:
		string += (str(i)+ ' ')
	
	print(string)
		
		
array = generate(8)

printArr(array)


