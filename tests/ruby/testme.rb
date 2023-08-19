=begin 
Ruby program to check whether the given number is 
prime or not.
=end

puts "Enter the number:"
num=gets.chomp.to_i
count=0
if (num==0)
	puts "0 is not prime"
else
	
	i=2
	while(i<num)
		if (num%i==0)
			count+=1
		end
		i+=1
	end
	
end
	if count>1
		puts "#{num is not a prime number"
	else
		puts "#{num} is a prime number"
	end
