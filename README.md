# All the Airwindows

(todo: description)


## Updating the airwindows sub-library

To update the airwindows library

1. Pull to the latest airwindows plugins

```bash
cd libs/airwindows
git pull origin master
cd ../..
```

2. For new plugins add them to `fxconfig.in`
3. run `scripts/configure.pl`
4. Do a test build
5. Commit src libs and the infile and push to github
6. voila

