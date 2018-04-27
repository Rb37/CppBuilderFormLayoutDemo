//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "algorithm.h"
#include "GridLayout.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

const size_t DEFAULT_CELL_MARGIN = 3;

struct TLayoutCell
{
    TLayoutCell()
        :   control(NULL),
            isMergedLeft(false),
            isMergedRight(false),
            isMergedTop(false),
            isMergedBottom(false)
    {};

    TControl* control;
    bool isMergedLeft;
    bool isMergedRight;
    bool isMergedTop;
    bool isMergedBottom;
};


TGridLayout::TGridLayout(TWinControl* aParent, size_t aColCount, size_t aRowCount)
    :   ALayout(aParent),
        colCount(aColCount),
        rowCount(aRowCount),
        cells(NULL),
        rowDefs(NULL),
        colDefs(NULL),
        cellMargin(DEFAULT_CELL_MARGIN)
{
    if (GetParent() == NULL || aColCount <= 0 || aRowCount <= 0)
        throw new LayoutException("Invalid argument");//Argument invalide
        
    AllocateMembers();
}

TGridLayout::~TGridLayout()
{
    FreeMembers();
}

void TGridLayout::AllocateMembers()
{
    FreeMembers();

    cells = new TLayoutCell*[colCount];
    for (size_t col = 0; col < colCount; ++col)
    {
        cells[col] = new TLayoutCell[rowCount];
    }

    rowDefs = new TRowDef[rowCount];
    colDefs = new TColDef[colCount];
}

void TGridLayout::FreeMembers()
{
    if (cells != NULL)
    {
        for (size_t col = 0; col < colCount; ++col)
        {
            delete[] cells[col];
        }
        delete[] cells;
        cells = NULL;
    }

    if (rowDefs != NULL)
    {
        delete[] rowDefs;
        rowDefs = NULL;
    }

    if (colDefs != NULL)
    {
        delete[] colDefs;
        colDefs = NULL;
    }
}

TControl* TGridLayout::GetControl(size_t col, size_t row)
{
    if (col >= colCount || row >= rowCount)
        return NULL;

    return cells[col][row].control;
}

void TGridLayout::SetControl(size_t col, size_t row, TControl* aControl)
{
    if (aControl != NULL && aControl->Parent != GetParent())
        throw new LayoutException("The parent of the controls must be identical to that of the layout");//Le parent des contrôles doit être identique à celui du layout

    cells[col][row].control = aControl;
}

void TGridLayout::ComputeColsWidth()
{
    // On veut calculer la hauteur moyenne (en pourcentage)
    // des lignes qui n'ont pas de hauteur spécifiée, ni de contrainte sur cette hauteur

    // We want to calculate the average height (in percentage)
    // lines that do not have a specified height, or any constraint on that height
    double defaultColWidthPercent = 1.0 / (double)colCount;
    int defaultColWidth = defaultColWidthPercent * GetRect().Width();

    // 1. Détection des contraintes de largeur de colonne non respectées
    // 1. Detection of column width constraints not respected
    size_t violationCount = 0;
    int nbEssaisResolution = 0;
    do
    {
        violationCount = 0;
        int constrainedWidth = 0; // largeur minimum requise pour respecter toutes les contraintes
        // minimum width required to meet all constraints
        for (size_t col = 0; col < colCount; ++col)
        {
            TColDef& colDef = colDefs[col];
            if (colDef.fixedWidth != DEFAULT_SIZE)
            {
                constrainedWidth += colDef.fixedWidth;
                colDef.width = colDef.fixedWidth;
                violationCount++;
            }
            else if (colDef.minWidth != DEFAULT_SIZE && defaultColWidth < colDef.minWidth)
            {
                constrainedWidth += colDef.minWidth;
                colDef.width = colDef.minWidth;
                violationCount++;
            }
            else if (colDef.maxWidth != DEFAULT_SIZE && defaultColWidth > colDef.maxWidth)
            {
                constrainedWidth += colDef.maxWidth;
                colDef.width = colDef.maxWidth;
                violationCount++;
            }
            else
            {
                colDef.width = DEFAULT_SIZE;
            }
        }

        if (constrainedWidth > 0 && GetRect().Width() > constrainedWidth && violationCount < colCount)
        {
            // 2. Ajustement de la largeur des colonnes non contraintes
            // 2. Adjusting the width of unconstrained columns
            defaultColWidth = (GetRect().Width() - constrainedWidth) / (colCount - violationCount);
            defaultColWidthPercent = (double)defaultColWidth / (double)GetRect().Width();
        }
    } while (violationCount > 0 && nbEssaisResolution++ < 3);

    // 3. Mise à jour de la largeur des colonnes non contraintes
    // 3. Updating the width of unconstrained columns
    int left = GetRect().left;
    for (size_t col = 0; col < colCount; ++col)
    {
        TColDef& colDef = colDefs[col];
        colDef.left = left;
        if (colDef.width == DEFAULT_SIZE)
            colDef.width = defaultColWidth;

        left += colDef.width;
    }
}

void TGridLayout::ComputeRowsHeight()
{
    // Pour calculer la hauteur de chaque ligne, il faut tenir compte de la hauteur moyenne
    // estimée, et des contraintes de tailles applicables.
    // Si la hauteur moyenne ne respecte pas les contraintes d'une ligne, la contrainte doit être respectée,
    // et une nouvelle hauteur moyenne calculée pour les autres lignes.
    // Cette nouvelle hauteur moyenne peut violer d'autres contraintes, le processus
    // est donc répété.
    // La satisfaction de toutes les contraintes peut être impossible (ex : hauteur d'écran insuffisante
    // pour satisfaire toutes les contraintes de hauteur minimale), d'ou une limitation sur le nombre de tentatives de résolution.

    // To calculate the height of each line, consider the average height
    // estimated, and applicable size constraints.
    // If the average height does not respect the constraints of a line, the constraint must be respected,
    // and a new average height calculated for the other lines.
    // This new average height can violate other constraints, so the process is repeated.
    // The satisfaction of all the constraints may be impossible (eg insufficient screen height
    // to satisfy all minimum height constraints), hence a limitation on the number of retry attempts.
    double defaultRowHeightPercent = 1.0 / (double)rowCount;
    int defaultRowHeight = defaultRowHeightPercent * GetRect().Height();

    // 1. Détection des contraintes de hauteur de ligne non respectées
    // 1. Detection of line height constraints not respected
    size_t violationCount = 0;
    int nbEssaisResolution = 0;
    do
    {
        violationCount = 0;
        int constrainedHeight = 0; // hauteur manquante, en pixels, pour respecter toutes les contraintes
        // missing height, in pixels, to respect all constraints
        for (size_t row = 0; row < rowCount; ++row)
        {
            TRowDef& rowDef = rowDefs[row];
            if (rowDef.fixedHeight != DEFAULT_SIZE)
            {
                constrainedHeight += rowDef.fixedHeight;
                rowDef.height = rowDef.fixedHeight;
                violationCount++;
            }
            else if (rowDef.minHeight != DEFAULT_SIZE && defaultRowHeight < rowDef.minHeight)
            {
                constrainedHeight += rowDef.minHeight;
                rowDef.height = rowDef.minHeight;
                violationCount++;
            }
            else if (rowDef.maxHeight != DEFAULT_SIZE && defaultRowHeight > rowDef.maxHeight)
            {
                constrainedHeight += rowDef.maxHeight;
                rowDef.height = rowDef.maxHeight;
                violationCount++;
            }
            else
            {
                rowDef.height = DEFAULT_SIZE;
            }
        }

        if (constrainedHeight != 0 && GetRect().Height() > constrainedHeight && violationCount < rowCount)
        {
            // Ajustement de la hauteur des lignes non contraintes
            // Adjust the height of unconstrained lines
            defaultRowHeight = (GetRect().Height() - constrainedHeight) / (rowCount - violationCount);
            defaultRowHeightPercent = (double)defaultRowHeight / (double)GetRect().Height();
        }
    } while (violationCount > 0 && nbEssaisResolution++ < 3);

    // 3. Mise à jour de la hauteur des lignes non contraintes
    // 3. Update the height of unstressed lines
    int top = GetRect().top;
    for (size_t row = 0; row < rowCount; ++row)
    {
        TRowDef& rowDef = rowDefs[row];
        rowDef.top = top;
        if (rowDef.height == DEFAULT_SIZE)
            rowDef.height = defaultRowHeight;

        top += rowDef.height;
    }
}

bool TGridLayout::IsVisible(size_t col, size_t row) const
{
    TLayoutCell& cell = cells[col][row];
    // Les cellules fusionnése avec leurs voisines de gauche ou du haut ne sont pas affichées
    // Cells merged with their neighbors on the left or top are not displayed
    return !cell.isMergedLeft && !cell.isMergedTop;
}

TRect TGridLayout::GetControlRect(size_t col, size_t row) const
{
    TRect cellRect(colDefs[col].left, rowDefs[row].top, colDefs[col].Right(), rowDefs[row].Bottom());
    for (size_t iCol = col; iCol+1 < colCount && cells[iCol][row].isMergedRight; ++iCol)
    {
        cellRect.Right = colDefs[iCol+1].Right();
    }
    for (size_t iRow = row; iRow < rowCount && cells[col][iRow].isMergedBottom; ++iRow)
    {
        cellRect.Bottom = rowDefs[iRow+1].Bottom();
    }

    // On retranche la marge intérieure des cellules
    // We subtract the internal margin of the cells
    cellRect.Left += cellMargin;
    cellRect.Right -= cellMargin;
    cellRect.Top += cellMargin;
    cellRect.Bottom -= cellMargin;

    return cellRect;
}

void TGridLayout::DoAlign()
{
    ComputeColsWidth();
    ComputeRowsHeight();
    for (size_t col = 0; col < colCount; ++col)
    {
        for (size_t row = 0; row < rowCount; ++row)
        {
            TControl* control = GetControl(col, row);
            if (control != NULL)
            {
                control->Visible = IsVisible(col, row);
                if (control->Visible)
                {
                    TRect cellRect = GetControlRect(col, row);
                    control->SetBounds(cellRect.Left, cellRect.Top, cellRect.Width(), cellRect.Height());
                }
            }
        }
    }
}

TGridLayout::TColDef* TGridLayout::ColDef(size_t col)
{
    if (col >= colCount)
        return NULL;

    return &colDefs[col];
}

TGridLayout::TRowDef* TGridLayout::RowDef(size_t row)
{
    if (row >= rowCount)
        return NULL;

    return &rowDefs[row];
}

void TGridLayout::MergeCells(size_t fromCol, size_t fromRow, size_t toCol, size_t toRow)
{
    if (fromCol >= toCol && fromRow >= toRow)
        return;

    // Ajustement des limites
    // Adjustment of limits
    fromCol = max((size_t)0, fromCol);
    toCol = min(toCol, colCount - 1);
    fromRow = max((size_t)0, fromRow);
    toRow = min(toRow, rowCount - 1);

    for (size_t col = fromCol; col <= toCol; ++col)
    {
        for (size_t row = fromRow; row <= toRow; ++row)
        {
            // La cellule est marquée fusionnée avec sa voisine de droite
            // The cell is marked merged with its neighbor on the right
            if (col < toCol)
            {
                cells[col][row].isMergedRight = true;
                // La voisine de droite est marquée fusionnée avec la cellule courante
                // The neighbor on the right is marked merged with the current cell
                cells[col+1][row].isMergedLeft = true;
            }

            // La cellule est marquée fusionnée avec sa voisine du bas
            // The cell is marked merged with its neighbor from the bottom
            if (row < toRow)
            {
                cells[col][row].isMergedBottom = true;
                // La voisine du bas est marquée fusionnée avec la cellule courante
                // The next neighbor is marked merged with the current cell
                cells[col][row+1].isMergedTop = true;
            }
        }
    }
}


void TGridLayout::FillParentHeight(TControl* topControl, TControl* bottomControl, int maxHeight)
{
    TGridLayout layout(topControl->Parent, 1, 2);
    layout.SetControl(0, 0, topControl);
    layout.SetControl(0, 1, bottomControl);

    TRect rect = topControl->Parent->ClientRect;
    if (maxHeight != DEFAULT_SIZE)
        rect.Bottom = rect.Top + maxHeight;

    layout.Align(rect);
}

void TGridLayout::FillParentWidth(TControl* leftControl, TControl* rightControl, int maxWidth)
{
    TGridLayout layout(leftControl->Parent, 2, 1);
    layout.SetControl(0, 0, leftControl);
    layout.SetControl(1, 0, rightControl);

    TRect rect = leftControl->Parent->ClientRect;
    if (maxWidth != DEFAULT_SIZE)
        rect.Right = rect.Left + maxWidth;

    layout.Align(rect);
}
