# tools-pocolog_pybind
Python bindings for rock tools-pocolog_cpp
This repository provides Python bindings for the pocolog tools [https://github.com/rock-core/tools-pocolog_cpp](tools-pocolog_cpp). As it relies on the mentioned `tools-pocolog_cpp` component, these Python bindings require a full rock installation. This repository needs to be cloned into `~/rock/tools/pocolog_pybind`.

Assuming the home folder corresponds to `/home/user`, we can install the Python bindings:
```
python3 -m pip install /home/user/rock/tools/pocolog_pybind
```

Below you can find a minimal example to use the Python bindings to read-in a pocolog log:
```python
import pocolog_pybind

# create file index. Its possible to specify multiple logfiles
multi_file_index = pocolog_pybind.pocolog.MultiFileIndex()
multi_file_index.create_index(["/home/user/rock/bundles/hdpr/logs/20210126-0820/ga_slam.0.log])

# we gather all available streams and put them into a dictionary using their names as keys
streams = self.multi_file_index.get_all_streams()
dict_streams = {}
for stream in streams:
    self.streams[stream.get_name()] = stream

# we loop through the /ga_slam.localElevationMapMean stream
stream = streams["/ga_slam.localElevationMapMean"]
print("type", stream.get_type_name())
print("size", stream.get_size())
print("first_sample_time", stream.get_first_sample_time())
print("last_sample_time", stream.get_last_sample_time())
for t in range(stream.get_size())):
    value = pocolog_pybind.pocolog.get_sample(stream, t)
```