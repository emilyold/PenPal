# TinySearchEngine

This project has three components. Each component has its own README that provides more details. A brief outline of the components, and the order they should be run in...

(1) crawler -- starts at a user-specified url and crawls to through links to a user-specified depth. HTML of all webpages is stored in text files in a user-specified directory.

(2) indexer -- organizes information about the HTML files created by the crawler using a hashtable that uses chaining for collisions 

(3) query engine -- takes in users' queries of words and/or phrases. Uses the index created by indexer to return the most relevent webpages containing the query that were crawled by the crawler. 
