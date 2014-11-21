import os

rootDir = os.path.abspath("..")

#Add to these lists where to call scons
targets = {}

#numbers in key give order

targets['1seth_common'] = os.path.join(rootDir, "dependencies")

targets["2ctsn_common"] = os.path.join(rootDir, "common")

targets["3picture_parser"] = os.path.join(rootDir, "picture_parser")

targets["4gateway"] = os.path.join(rootDir,"gateway")

