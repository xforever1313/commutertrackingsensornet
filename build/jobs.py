import os

rootDir = os.path.abspath("..")

#Add to these lists where to call scons
targets = {}

#numbers in key give order

targets['1seth_common'] = os.path.join(rootDir, "dependencies")

targets["2ctsn_common"] = os.path.join(rootDir, "common")

targets["3gateway"] = os.path.join(rootDir,"gateway")

targets["4pi_node"] = os.path.join(rootDir, "pi_node")

