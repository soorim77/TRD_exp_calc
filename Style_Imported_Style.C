// Mainframe macro generated from application: /home/soorim/root/bin/root.exe
// By ROOT version 6.20/04 on 2020-10-04 16:43:02

#if !defined( __CINT__) || defined (__MAKECINT__)

#ifndef ROOT_TStyle
#include "TStyle.h"
#endif

#endif

void Style_Imported_Style()
{
   // Add the saved style to the current ROOT session.

   delete gROOT->GetStyle("Imported_Style");

   TStyle *tmpStyle = new TStyle("Imported_Style", "Imported from canvas canv_spot_contour");
   tmpStyle->SetNdivisions(510, "x");
   tmpStyle->SetNdivisions(510, "y");
   tmpStyle->SetNdivisions(510, "z");
   tmpStyle->SetAxisColor(1, "x");
   tmpStyle->SetAxisColor(1, "y");
   tmpStyle->SetAxisColor(1, "z");
   tmpStyle->SetLabelColor(1, "x");
   tmpStyle->SetLabelColor(1, "y");
   tmpStyle->SetLabelColor(1, "z");
   tmpStyle->SetLabelFont(42, "x");
   tmpStyle->SetLabelFont(42, "y");
   tmpStyle->SetLabelFont(42, "z");
   tmpStyle->SetLabelOffset(0.005, "x");
   tmpStyle->SetLabelOffset(0.005, "y");
   tmpStyle->SetLabelOffset(0.005, "z");
   tmpStyle->SetLabelSize(0.05, "x");
   tmpStyle->SetLabelSize(0.05, "y");
   tmpStyle->SetLabelSize(0.035, "z");
   tmpStyle->SetTickLength(0.03, "x");
   tmpStyle->SetTickLength(0.03, "y");
   tmpStyle->SetTickLength(0.03, "z");
   tmpStyle->SetTitleOffset(1, "x");
   tmpStyle->SetTitleOffset(0, "y");
   tmpStyle->SetTitleOffset(1, "z");
   tmpStyle->SetTitleSize(0.05, "x");
   tmpStyle->SetTitleSize(0.05, "y");
   tmpStyle->SetTitleSize(0.035, "z");
   tmpStyle->SetTitleColor(1, "x");
   tmpStyle->SetTitleColor(1, "y");
   tmpStyle->SetTitleColor(1, "z");
   tmpStyle->SetTitleFont(42, "x");
   tmpStyle->SetTitleFont(42, "y");
   tmpStyle->SetTitleFont(42, "z");
   tmpStyle->SetBarWidth(1);
   tmpStyle->SetBarOffset(0);
   tmpStyle->SetDrawBorder(0);
   tmpStyle->SetOptLogx(0);
   tmpStyle->SetOptLogy(0);
   tmpStyle->SetOptLogz(0);
   tmpStyle->SetOptDate(0);
   tmpStyle->SetOptStat(1111);
   tmpStyle->SetOptTitle(kTRUE);
   tmpStyle->SetOptFit(0);
   tmpStyle->SetNumberContours(20);
   tmpStyle->GetAttDate()->SetTextFont(62);
   tmpStyle->GetAttDate()->SetTextSize(0.025);
   tmpStyle->GetAttDate()->SetTextAngle(0);
   tmpStyle->GetAttDate()->SetTextAlign(11);
   tmpStyle->GetAttDate()->SetTextColor(1);
   tmpStyle->SetDateX(0.01);
   tmpStyle->SetDateY(0.01);
   tmpStyle->SetEndErrorSize(2);
   tmpStyle->SetErrorX(0.5);
   tmpStyle->SetFuncColor(2);
   tmpStyle->SetFuncStyle(1);
   tmpStyle->SetFuncWidth(2);
   tmpStyle->SetGridColor(0);
   tmpStyle->SetGridStyle(3);
   tmpStyle->SetGridWidth(1);
   tmpStyle->SetLegendBorderSize(1);
   tmpStyle->SetLegendFillColor(0);
   tmpStyle->SetLegendFont(42);
   tmpStyle->SetLegendTextSize(0);
   tmpStyle->SetHatchesLineWidth(1);
   tmpStyle->SetHatchesSpacing(1);
   tmpStyle->SetFrameFillColor(0);
   tmpStyle->SetFrameLineColor(1);
   tmpStyle->SetFrameFillStyle(1001);
   tmpStyle->SetFrameLineStyle(1);
   tmpStyle->SetFrameLineWidth(1);
   tmpStyle->SetFrameBorderSize(1);
   tmpStyle->SetFrameBorderMode(0);
   tmpStyle->SetHistFillColor(0);
   tmpStyle->SetHistLineColor(602);
   tmpStyle->SetHistFillStyle(1001);
   tmpStyle->SetHistLineStyle(1);
   tmpStyle->SetHistLineWidth(1);
   tmpStyle->SetHistMinimumZero(kFALSE);
   tmpStyle->SetCanvasPreferGL(kFALSE);
   tmpStyle->SetCanvasColor(0);
   tmpStyle->SetCanvasBorderSize(2);
   tmpStyle->SetCanvasBorderMode(0);
   tmpStyle->SetCanvasDefH(500);
   tmpStyle->SetCanvasDefW(700);
   tmpStyle->SetCanvasDefX(10);
   tmpStyle->SetCanvasDefY(10);
   tmpStyle->SetPadColor(0);
   tmpStyle->SetPadBorderSize(2);
   tmpStyle->SetPadBorderMode(0);
   tmpStyle->SetPadBottomMargin(0.13);
   tmpStyle->SetPadTopMargin(0.15);
   tmpStyle->SetPadLeftMargin(0.13);
   tmpStyle->SetPadRightMargin(0.13);
   tmpStyle->SetPadGridX(kFALSE);
   tmpStyle->SetPadGridY(kFALSE);
   tmpStyle->SetPadTickX(0);
   tmpStyle->SetPadTickY(0);
   tmpStyle->SetPaperSize(20, 26);
   tmpStyle->SetScreenFactor(1);
   tmpStyle->SetStatColor(0);
   tmpStyle->SetStatTextColor(1);
   tmpStyle->SetStatBorderSize(1);
   tmpStyle->SetStatFont(42);
   tmpStyle->SetStatFontSize(0);
   tmpStyle->SetStatStyle(1001);
   tmpStyle->SetStatFormat("6.4g");
   tmpStyle->SetStatX(0.98);
   tmpStyle->SetStatY(0.935);
   tmpStyle->SetStatW(0.2);
   tmpStyle->SetStatH(0.16);
   tmpStyle->SetStripDecimals(kTRUE);
   tmpStyle->SetTitleAlign(23);
   tmpStyle->SetTitleFillColor(0);
   tmpStyle->SetTitleTextColor(1);
   tmpStyle->SetTitleBorderSize(0);
   tmpStyle->SetTitleFont(42);
   tmpStyle->SetTitleFontSize(0.05);
   tmpStyle->SetTitleStyle(0);
   tmpStyle->SetTitleX(0.5);
   tmpStyle->SetTitleY(0.995);
   tmpStyle->SetTitleW(0);
   tmpStyle->SetTitleH(0);
   tmpStyle->SetLegoInnerR(0.5);

   Int_t fPaletteColor[255] = {1179, 1180, 1181, 1182, 1183, 1184, 1185, 1186, 1187, 
                             1188, 1189, 1190, 1191, 1192, 1193, 1194, 1195, 1196, 1197, 
                             1198, 1199, 1200, 1201, 1202, 1203, 1204, 1205, 1206, 1207, 
                             1208, 1209, 1210, 1211, 1212, 1213, 1214, 1215, 1216, 1217, 
                             1218, 1219, 1220, 1221, 1222, 1223, 1224, 1225, 1226, 1227, 
                             1228, 1229, 1230, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 
                             1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1246, 1247, 
                             1248, 1249, 1250, 1251, 1252, 1253, 1254, 1255, 1256, 1257, 
                             1258, 1259, 1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 
                             1268, 1269, 1270, 1271, 1272, 1273, 1274, 1275, 1276, 1277, 
                             1278, 1279, 1280, 1281, 1282, 1283, 1284, 1285, 1286, 1287, 
                             1288, 1289, 1290, 1291, 1292, 1293, 1294, 1295, 1296, 1297, 
                             1298, 1299, 1300, 1301, 1302, 1303, 1304, 1305, 1306, 1307, 
                             1308, 1309, 1310, 1311, 1312, 1313, 1314, 1315, 1316, 1317, 
                             1318, 1319, 1320, 1321, 1322, 1323, 1324, 1325, 1326, 1327, 
                             1328, 1329, 1330, 1331, 1332, 1333, 1334, 1335, 1336, 1337, 
                             1338, 1339, 1340, 1341, 1342, 1343, 1344, 1345, 1346, 1347, 
                             1348, 1349, 1350, 1351, 1352, 1353, 1354, 1355, 1356, 1357, 
                             1358, 1359, 1360, 1361, 1362, 1363, 1364, 1365, 1366, 1367, 
                             1368, 1369, 1370, 1371, 1372, 1373, 1374, 1375, 1376, 1377, 
                             1378, 1379, 1380, 1381, 1382, 1383, 1384, 1385, 1386, 1387, 
                             1388, 1389, 1390, 1391, 1392, 1393, 1394, 1395, 1396, 1397, 
                             1398, 1399, 1400, 1401, 1402, 1403, 1404, 1405, 1406, 1407, 
                             1408, 1409, 1410, 1411, 1412, 1413, 1414, 1415, 1416, 1417, 
                             1418, 1419, 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1427, 
                             1428, 1429, 1430, 1431, 1432, 1433};
   tmpStyle->SetPalette(255, fPaletteColor);

   TString fLineStyleArrayTmp[30] = {"", "  ", " 12 12", " 4 8", 
                             " 12 16 4 16", " 20 12 4 12", " 20 12 4 12 4 12 4 12", " 20 20", " 20 12 4 12 4 12", 
                             " 80 20", " 80 40 4 40", "  ", "  ", "  ", 
                             "  ", "  ", "  ", "  ", "  ", 
                             "  ", "  ", "  ", "  ", "  ", 
                             "  ", "  ", "  ", "  ", "  ", "  "};
   for (Int_t i=0; i<30; i++)
      tmpStyle->SetLineStyleString(i, fLineStyleArrayTmp[i]);

   tmpStyle->SetHeaderPS("");
   tmpStyle->SetTitlePS("");
   tmpStyle->SetFitFormat("5.4g");
   tmpStyle->SetPaintTextFormat("g");
   tmpStyle->SetLineScalePS(3);
   tmpStyle->SetJoinLinePS(0);
   tmpStyle->SetColorModelPS(0);
   tmpStyle->SetTimeOffset(788918400);

   tmpStyle->SetLineColor(1);
   tmpStyle->SetLineStyle(1);
   tmpStyle->SetLineWidth(1);
   tmpStyle->SetFillColor(19);
   tmpStyle->SetFillStyle(1001);
   tmpStyle->SetMarkerColor(1);
   tmpStyle->SetMarkerSize(1);
   tmpStyle->SetMarkerStyle(1);
   tmpStyle->SetTextAlign(11);
   tmpStyle->SetTextAngle(0);
   tmpStyle->SetTextColor(1);
   tmpStyle->SetTextFont(42);
   tmpStyle->SetTextSize(0);
}
