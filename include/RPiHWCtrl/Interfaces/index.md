Interfaces package
==================

The Interfaces package contains the interfaces used by the library to abstract
the different implementations and help for the modularization. These interfaces
are the following:

- `Input<T>` : Represents an input which provides values of type T
- `Output<T>` : Represents an output which can consume values of type T
- `Observer<T>` : Object that can be notified for events of type T
- `Observable<T>` : Object that generates events of type T

Note that all the interfaces are templated on the type of the value they handle,
which means that there is a set of interfaces for each type T. The type T is
restricted to only arithmetic types.