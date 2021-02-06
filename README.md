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
multi_file_index.create_index(["/home/user/rock/bundles/hdpr/logs/20210126-0820/ga_slam.0.log"])

# we gather all available streams and put them into a dictionary using their names as keys
streams = self.multi_file_index.get_all_streams()

# we loop through the /ga_slam.localElevationMapMean stream
stream = streams["/ga_slam.localElevationMapMean"]
print("type", stream.get_type_name())
print("size", stream.get_size())
print("first_sample_time", stream.get_first_sample_time())
print("last_sample_time", stream.get_last_sample_time())
for t in range(stream.get_size())):
    value = stream.get_sample(t)

    """
    print the structure of the Typelib::Value 
    The Typelib::Category is most likely a Container at the top-level of the stream thus we can print the structure of the container
    """
    self.pocolog_pybind.typelib.type_display(value.get_type(), "")

    """
    We cast the Typelib::Value object recursively into python types
        Typelib::Compound --> Python dict
        Typelib::Array --> Python list
        Typelib::Container --> Python string or array
        Typelib::Numeric --> corresponding numeric type in Python
    If we set recursively=False, only the first level is casted
    """

    py_value = value.cast(recursive=True)

    # now we can print the keys of the Python dictionary
    print(py_value.keys())

    # and access our local DEM
    local_dem = np.array(py_value["data"])
    local_dem = occ_dem.reshape((py_value["height"], py_value["width"]), order="F")

```