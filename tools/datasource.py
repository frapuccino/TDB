import struct
import sys
import argparse
import random
import string


# TODO(frapuccino): optimize data generation speed
if __name__ == "__main__":
  parser = argparse.ArgumentParser(description="datasource")
  parser.add_argument('-s','--size', default=(1 << 30))
  parser.add_argument('-e','--entry', default=(1 << 10))
  parser.add_argument('-o','--output', default='./data.bin')
  args = parser.parse_args()
  ag_dict = vars(args)

  file_size = int(ag_dict['size'])
  entry_size = int(ag_dict['entry'])
  output = ag_dict['output']
  print(entry_size)
  max_kv_size = entry_size * 2 + 16
  cur_size = 0
  fo = open(output, "wb+")
  while cur_size < file_size:
    kv_size = 0
    key_size = 0
    value_size = 0
    if cur_size + max_kv_size + 16 >= file_size:
      if cur_size + 18 > file_size:
        break
      kv_size = file_size - cur_size - 16
      key_size = random.randint(0, min(kv_size - 2, entry_size - 1)) + 1
      # key_size = random.randint(0, kv_size - 2) + 1
      value_size = min(random.randint(0, entry_size - 1) + 1, kv_size - key_size)
    else:
      key_size = random.randint(0, entry_size - 1) + 1
      value_size = random.randint(0, entry_size - 1) + 1

    cur_size += key_size + value_size + 16
    rand_key_list = [random.choice(string.digits + string.ascii_letters) for i in range(key_size)]
    rand_key = ''.join(rand_key_list)
    rand_key = rand_key.encode(encoding="utf-8")

    rand_value_list = [random.choice(string.digits + string.ascii_letters) for i in range(value_size)]
    rand_value = ''.join(rand_value_list)
    rand_value = rand_value.encode(encoding="utf-8")
    # little-endian
    stream=struct.pack('<Q'+str(key_size)+'sQ'+str(value_size)+'s',key_size,rand_key,value_size,rand_value)
    fo.write(stream)
  fo.close()
