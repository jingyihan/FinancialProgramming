#384. Suffle an Array

class Solution(object):

    import random
    
    def __init__(self, nums):
        """
        :type nums: List[int]
        """
        self.curr = nums
        self.orig = [nums[i] for i in range(len(nums))]
        

    def reset(self):
        """
        Resets the array to its original configuration and return it.
        :rtype: List[int]
        """
        self.curr = [self.orig[i] for i in range(len(self.orig))]
        return self.curr
        

    def shuffle(self):
        """
        Returns a random shuffling of the array.
        :rtype: List[int]
        """
        for i in range(len(self.curr)-1, -1, -1):
            r_index = random.randint(0, i)
            if (r_index != i):
                self.curr[r_index], self.curr[i] = self.curr[i], self.curr[r_index]
        return self.curr


# Your Solution object will be instantiated and called as such:
# obj = Solution(nums)
# param_1 = obj.reset()
# param_2 = obj.shuffle()