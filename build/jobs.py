import os

rootDir = os.path.abspath("..")

#Add to these lists where to call scons
targets = {}

#numbers in key give order

targets['1seth_common'] = os.path.join(rootDir, "dependencies")

targets["2ctsn_common"] = os.path.join(rootDir, "common")

targets["3libb64"] = os.path.join(rootDir, "dependencies/libb64")

targets["4picture_parser"] = os.path.join(rootDir, "picture_parser")

targets["5gateway"] = os.path.join(rootDir,"gateway")

targets["6pi_node"] = os.path.join(rootDir, "pi_node")

