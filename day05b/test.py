nums = [202517468,131640971,1553776977,241828580,1435322022,100369067,2019100043,153706556,460203450,84630899,3766866638,114261107,1809826083,153144153,2797169753,177517156,2494032210,235157184,856311572,542740109]
cur = 0
first = 0
second = 0
maximum = list() 
for i in nums:
    if cur == 0:
        first = i
    elif cur == 1:
        second = i
        maximum.append((first + second) - first)
        cur = 0
    cur += 1
print(max(maximum))
print(len(nums))
