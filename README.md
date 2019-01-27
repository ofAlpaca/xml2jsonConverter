# A xml and json foramt converter

## What can it do ?
This library depends on cJSON and ezxml, both of library are written in pure C. So do this converter.

Basically, this converter can transform most of the xml format file into json format, and vice versa **(in my opinion)**.

## How to set up ?
- To create the converter static library.
```
make
```
- To create two example executable files by the name of `test_json2xml` which convert json to xml and `test_xml2json` which convert xml to json on the other hand.
```
make test
```

## Know the limitation
While transforming a json into a xml, there will appear two tags which you may not familiar. 

1. `pseudoTag` tag is used to handle the most outside braces in json format. For example,
    ```
    {
        "name" : "Tom",
        "age"  : 23,
        "gender" : "Helicopter"
    }
    ```
    This will be converted to the following.
    ```
    <pseudoTag>
        <name>Tom</name>
        <age>23</age>
        <gender>Helicopter</gender>
    </pseudoTag>
    ```
2. `pseudoIndex` tag is used to deal with double array in json format. For examlpe,
    ```
    {
        "doubleArr" : [[[3,5,4]], [false, true, false]]
    }
    ```
    This will be converted to the following.
    ``` 
    <pseudoTag>
        <doubleArr>
            <pseudoIndex>
                <pseudoIndex>3.00</pseudoIndex>
                <pseudoIndex>5.00</pseudoIndex>
                <pseudoIndex>4.00</pseudoIndex>
            </pseudoIndex>
        </doubleArr>
        <doubleArr>
            <pseudoIndex>false</pseudoIndex>
            <pseudoIndex>true</pseudoIndex>
            <pseudoIndex>false</pseudoIndex>
        </doubleArr>
    </pseudoTag>
    ```
3. This library does't have any pretty print function in xml format, so you have to use some online tools to make it easier to read.