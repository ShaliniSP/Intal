from math import ceil, floor
#math.ceil(x) Return the ceiling of x as a float, the smallest integer value greater than or equal to x.
#math.floor(x) Return the floor of x as a float, the largest integer value less than or equal to x.

def karatsuba(x,y):

      #base case
      if x < 10 and y < 10: # in other words, if x and y are single digits
        return x*y

      n = max(len(str(x)), len(str(y)))
      m = int(ceil(float(n)/2))   #Cast n into a float because n might lie outside the representable range of integers.

      #print("fh: ", m)
      #print("lh: ", n-m)

      x_H  = int(floor(x / 10**m))
      #print('I1_L:' , x_H )
      x_L = int(x % (10**m))
      #print("I1_R:" , x_L )
      y_H = int(floor(y / 10**m))
      #print("I2_L:" , y_H )
      y_L = int(y % (10**m))
      #print("I2_R:" , y_L )

    #recursive steps
      a = karatsuba(x_H,y_H)
      print("P1:" , a)
      d = karatsuba(x_L,y_L)
      print("P2:" , d )
      print("P3:", karatsuba(x_H + x_L, y_H + y_L))
      e = karatsuba(x_H + x_L, y_H + y_L) -a -d
      #print("e: " , e , "\n")
      print("F:", a*(10**(m*2)))
      print("M:", e*(10**m))
      print("Prod:" , int(a*(10**(m*2)) + e*(10**m) + d))
      return int(a*(10**(m*2)) + e*(10**m) + d)



#print(karatsuba(12,12))
#print(karatsuba(0,0))
#print(karatsuba(1234,4321))
#print(karatsuba(3141592653589793238462643383279502884197169399375105820974944592,2718281828459045235360287471352662497757247093699959574966967627))
#print(karatsuba(3141592653589793238460092338,271828182845904523536))
#print(karatsuba(8765904321,1092837465))
#print(karatsuba(2823357918,302344))

