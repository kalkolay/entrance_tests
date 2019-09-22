import pandas as pd
import requests
from decimal import Decimal
import certifi
from collections import defaultdict

# create a .csv file with ETH wallets
"""
wallets = {"Wallet": ['0x4FED1fC4144c223aE3C1553be203cDFcbD38C581',
                      '0xE9B1a2164368c00FC93e0e749d9B3cAFA1bC6eE2',
                      '0x50756F9553c0f0f72a698a156A4A5D1C78E86276',
                      '0x7b9159E19517591a1b131944dEf9d01F3F81c96b',
                      '0x4E6E0b59CB9Dc03c6316E16f868B840b26809e3c',
                      '0xf0F1698B326fB8F7A3244260916Bb6441c08f891',
                      '0xC18CDF40CEEF1cD66Cdc3500EE844CfCf09145C1',
                      '0xD32d2Fe8de7C6910eF5c769961cF8E3680181c0f',
                      '0x4d26e0DD23c8eA52c1E1B8636fE735121e54FeFc',
                      '0x94A4d16BBda095aE917E1adb8647C70f9b9Adc76']
           }
walletsDF = pd.DataFrame(wallets)
walletsDF.to_csv("wallets.csv", index=False)
"""


# data of each transaction
class Transaction(object):
    def __init__(self, tx_hash, eth_value, status):
        self.tx_hash = tx_hash
        self.eth_value = eth_value
        self.status = status


# main function which parses all transactions in ETH wallet
def get_transactions_by_address(address, number_transactions):
    url = "https://api.etherscan.io/api?module=account&action=txlist&address=" + address + \
          "&startblock=0&endblock=99999999&page=1&offset=" + \
          str(number_transactions) + "&sort=desc&apikey=YourApiKey"

    response = requests.get(url, verify=certifi.where())
    address_content = response.json()
    result = address_content.get("result")

    data = defaultdict(dict)
    for i, transaction in enumerate(result):
        transaction_info = Transaction(transaction.get("hash"),
                                       float(Decimal(transaction.get("value")) /
                                             Decimal("1000000000000000000")),
                                       transaction.get("confirmations"))

        data[i]["tx_hash"] = transaction_info.tx_hash
        data[i]["eth_value"] = transaction_info.eth_value
        data[i]["status"] = transaction_info.status
    return data


# create a data frame with ETH wallets
walletsDF = pd.read_csv("wallets.csv")
size = len(walletsDF.index)

# create a constant for maximal number of transactions
MAX_TX_COUNT = 1000

# parse each wallet's address from the data frame
walletAddress = [None] * size
for i in range(size):
    walletAddress[i] = walletsDF.iloc[i]['Wallet']

# parse all transactions in each wallet
tx_data = [None] * size
for i in range(size):
    tx_data[i] = get_transactions_by_address(walletAddress[i], MAX_TX_COUNT)

# save transactions of all wallets
df = [None] * size
for i in range(size):
    df[i] = pd.DataFrame(tx_data[i])
    df[i].to_csv("result/wallet" + str(i + 1) + ".csv")
