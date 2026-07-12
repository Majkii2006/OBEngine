import matplotlib.pyplot as plt

# Dane
quantity = [10, 100, 1000, 10000, 1000000]

market_buy = [56, 85, 119, 86, 80]
market_sell = [96, 112, 83, 99, 127]
limit_buy = [67, 85, 82, 87, 77]
limit_sell = [127, 179, 176, 142, 118]

# Styl wykresu
plt.style.use("seaborn-v0_8-darkgrid")

plt.figure(figsize=(10, 6))

# Linie
plt.plot(quantity, market_buy,
         marker='o', linewidth=2.5,
         color='#1f77b4', label='Market Buy')

plt.plot(quantity, market_sell,
         marker='s', linewidth=2.5,
         color='#ff7f0e', label='Market Sell')

plt.plot(quantity, limit_buy,
         marker='^', linewidth=2.5,
         color='#2ca02c', label='Limit Buy')

plt.plot(quantity, limit_sell,
         marker='D', linewidth=2.5,
         color='#d62728', label='Limit Sell')

# Skala logarytmiczna
plt.xscale("log")

# Opisy
plt.title("Execution Time vs Order Quantity", fontsize=16, weight="bold")
plt.xlabel("Order Quantity", fontsize=12)
plt.ylabel("Execution Time [us]", fontsize=12)

# Legenda
plt.legend(fontsize=11)

# Siatka
plt.grid(True, which="both", linestyle="--", alpha=0.5)

plt.tight_layout()
plt.show()
