from typing import List


class MaxProf:
    """A class to calculate the maximum profit from a list of stock prices.

    This class provides functionality to calculate the maximum profit that can be
    obtained by buying and selling stocks multiple times.
    """

    def maxProfit(self, prices: List[float]) -> float:
        """
        calcs max prof from buying & selling stocks multiple times

        Args:
            prices: A list of stock prices where prices[i] is the price on day i.

        Returns:
            float: Max prof
        """
        # NOTE: use a greedy algo instead of dynamic programming as there is no
        # performance benefit over greedy. If today's price is higher than
        # yesterday's, add the diff to profit.
        # O(n) time complexity, O(1) space complexity
        profit = 0.0
        for i in range(1, len(prices)):
            if prices[i] > prices[i - 1]:
                profit += prices[i] - prices[i - 1]
        return profit
